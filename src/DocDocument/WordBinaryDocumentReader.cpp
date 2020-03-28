#include <SDRL/DocDocument/WordBinaryDocumentReader.hpp>
#include <SDRL/DocDocument/variablevisualize.hpp>
#include <SDRL/DocDocument/WordBinaryFileFormat/wbff_structures.h>
#include <SDRL/DocDocument/WordBinaryFileFormat/wbff_readers.hpp>

namespace sdrl {

	size_t calcPlcElementsNumber( uint32_t structureSize
								, uint32_t elementSize );

	struct WordBinaryDocumentReader::FIB_Begin
	{
		FIB_Base fibBase;
		FIB_RgW97 fibRgW97;
		FIB_RgLw97 fibLw97;
	};

	WordBinaryDocumentReader::WordBinaryDocumentReader(const std::wstring& fileName, uint16_t flags)
		: WindowsCompoundBinaryFileFormatReader(fileName), _flags(flags)
	{
		readDocumentHeader();
	}

	WordBinaryDocumentReader::WordBinaryDocumentReader(std::istream& stream, uint16_t flags)
		: WindowsCompoundBinaryFileFormatReader(stream), _flags(flags)
	{
		readDocumentHeader();
	}


	uint16_t WordBinaryDocumentReader::formatVersion()
	{
		switch (_fibBegin->fibBase.fileFormatVersion)
		{
		case 0x00C1: return 1997;
		case 0x00D9: return 2000;
		case 0x0101: return 2002;
		case 0x010C: return 2003;
		case 0x0112: return 2007;
		}

		return 0;
	}

	void WordBinaryDocumentReader::readDocumentHeader()
	{
		_wordDocumentStream = std::make_shared<InputBinaryStream<char>>(getStream("WordDocument"));

		_fibBegin = std::make_shared<FIB_Begin>();

		_wordDocumentStream->base() >> _fibBegin->fibBase;
		_wordDocumentStream->ignore(sizeof(uint16_t));
		_wordDocumentStream->base() >> _fibBegin->fibRgW97;
		_wordDocumentStream->ignore(sizeof(uint16_t));
		_wordDocumentStream->base() >> _fibBegin->fibLw97;

		auto fibRgFcLcbSize = _wordDocumentStream->getData<uint16_t>();
		auto cswNew = _wordDocumentStream->peekData<uint16_t>(154 + fibRgFcLcbSize * 8);
		if (cswNew) {
			_fibBegin->fibBase.fileFormatVersion = _wordDocumentStream->peekData<uint16_t>(156 + fibRgFcLcbSize * 8);
		}

		std::string nameofTableStream("0Table");
		if ( _fibBegin->fibBase.hasStreamTableOne )
		{
			nameofTableStream = "1Table";
		}

		_tableStream = std::make_shared<InputBinaryStream<char>>(getStream(nameofTableStream));

		switch (formatVersion())
		{
		case 1997: {
			_fibEnd = std::make_shared<FIB_RgFcLcb97>();
			*_fibEnd = _wordDocumentStream->getData<FIB_RgFcLcb97>();
		} break;
		case 2000: {
			_fibEnd = std::make_shared<FIB_RgFcLcb2000>();
			*_fibEnd = _wordDocumentStream->getData<FIB_RgFcLcb2000>();
		} break;
		case 2002: {
			_fibEnd = std::make_shared<FIB_RgFcLcb2002>();
			*_fibEnd = _wordDocumentStream->getData<FIB_RgFcLcb2002>();
		} break;
		case 2003: {
			_fibEnd = std::make_shared<FIB_RgFcLcb2003>();
			*_fibEnd = _wordDocumentStream->getData<FIB_RgFcLcb2003>();
		} break;
		case 2007: {
			_fibEnd = std::make_shared<FIB_RgFcLcb2007>();
			*_fibEnd = _wordDocumentStream->getData<FIB_RgFcLcb2007>();
		} break;
		}
	}

	std::istream& WordBinaryDocumentReader::getStream(std::string streamName)
	{
		auto streamID = this->getStreamIdByName(streamName);
		if (streamID < 0)
		{
			throw std::runtime_error("Stream \"" + streamName + "\" not found!");
		}

		return this->getStreamById(streamID);
	}

	/* ///////////////////////////////////////////////////

	Read docInfo

	/////////////////////////////////////////////////// */

	std::shared_ptr<Dop> WordBinaryDocumentReader::documentInfo()
	{
		auto dop = std::make_shared<Dop>();

		_tableStream->goTo(_fibEnd->fcDop + 6);

		auto flags = _tableStream->getData<uint8_t>();
		dop->fExactCWords = (flags >> 1) & 0x1;

		_tableStream->goTo(_fibEnd->fcDop + 20);
		dop->dttmCreated = _tableStream->getData<uint32_t>();
		dop->dttmRevised = _tableStream->getData<uint32_t>();
		_tableStream->ignore(sizeof(uint32_t));

		dop->nRevision = _tableStream->getData<uint16_t>();
		dop->tmEdited = _tableStream->getData<int32_t>();
		dop->cWords = _tableStream->getData<int32_t>();
		dop->cCh = _tableStream->getData<int32_t>();
		dop->cPg = _tableStream->getData<int16_t>();
		dop->cParas = _tableStream->getData<int32_t>();
		_tableStream->ignore(sizeof(uint32_t));
		dop->cLines = _tableStream->getData<int32_t>();

		return dop;
	}

	/* ///////////////////////////////////////////////////

	Read text

	/////////////////////////////////////////////////// */

	struct _TextData {
		InputBinaryStream<char>& tableStream;
		FIB_RgLw97& fibLw;
		std::vector<uint32_t> charactersPositions;
		std::vector<Pcd> charactersOffsets;
	};

	void readClxArray(_TextData& data);
	void readPrcArray(_TextData& data);
	void readPrcDataArray(_TextData& data);
	void readPrlArray(_TextData& data, int16_t prlArraySize);
	void readPlcArray(_TextData& data);
	void readPcdtArray(_TextData& data);
	void readPlcPcdArray(_TextData& data, uint32_t plcPcdSize);
	void readCpArray(_TextData& data, uint32_t lastCpValue);
	void readCharacters(_TextData& data, std::istream& document, std::wstringstream& output);

	std::wstring WordBinaryDocumentReader::text()
	{
		_TextData data{
			*_tableStream,
			_fibBegin->fibLw97
		};

		_tableStream->base().seekg(_fibEnd->fcClx, _tableStream->base().beg);

		std::wstringstream resultBuffer;

		readClxArray(data);
		readCharacters(data, _wordDocumentStream->base(), resultBuffer);

		return resultBuffer.str();
	}

	void readClxArray(_TextData& data)
	{
		readPrcArray(data);
		readPcdtArray(data);
	}

	void readPrcArray(_TextData& data)
	{
		while (data.tableStream.peekData<uint8_t>() == 0x01)
		{
			data.tableStream.getData<uint8_t>();
			readPrcDataArray(data);
		}
	}

	void readPrcDataArray(_TextData& data)
	{
		auto cbGrpprl = data.tableStream.getData<int16_t>();
		readPrlArray(data, cbGrpprl);
	}

	void readPrlArray(_TextData& data, int16_t prlArraySize)
	{
		static const uint16_t bufferSize = 1024;
		static char buffer[bufferSize];

		// Заглушка
		for (uint16_t needReadBytes = prlArraySize; needReadBytes > 0;)
		{
			uint16_t bytesAmountForRead = needReadBytes < bufferSize ? needReadBytes : bufferSize;
			needReadBytes -= bytesAmountForRead;

			data.tableStream.base().read(buffer, bytesAmountForRead);
		}
	}

	void readPcdtArray(_TextData& data)
	{
		while (data.tableStream.peekData<uint8_t>() == 0x02)
		{
			data.tableStream.getData<uint8_t>();
			readPlcPcdArray(data, data.tableStream.getData<uint32_t>());
		}
	}

	void readPlcPcdArray(_TextData& data, uint32_t plcPcdSize)
	{
		data.charactersPositions.reserve(plcPcdSize / sizeof(uint32_t));

		uint32_t lastCP = data.fibLw.ccpText;
		uint32_t ccpSumm = data.fibLw.ccpAtn + data.fibLw.ccpEdn
			+ data.fibLw.ccpFtn + data.fibLw.ccpHdd
			+ data.fibLw.ccpHdrTxbx + data.fibLw.ccpTxbx;
		if (ccpSumm > 0) {
			lastCP += ccpSumm + 1;
		}

		readCpArray(data, lastCP);
		readPlcArray(data);
	}

	void readCpArray(_TextData& data, uint32_t lastCpValue)
	{
		uint32_t cpOffset = 0;
		do
		{
			cpOffset = data.tableStream.getData<uint32_t>();
			data.charactersPositions.push_back(cpOffset);
		} while (cpOffset != lastCpValue);
	}

	void readPlcArray(_TextData& data)
	{
		data.charactersOffsets.reserve(data.charactersPositions.size());

		for (uint32_t i = 1; i < data.charactersPositions.size(); i++)
		{

			auto firstDataBlock = data.tableStream.getData<uint16_t>();
			auto fcCompresdsedDataBlock = data.tableStream.getData<uint32_t>();
			auto prmDataBlock = data.tableStream.getData<uint16_t>();

			Pcd pcd;
			pcd.A_fNoParaLast = firstDataBlock & 0x1;
			pcd.B_fR1 = (firstDataBlock >> 1) & 0x1;
			pcd.C_fDirty = (firstDataBlock >> 2) & 0x1;

			pcd.fc = fcCompresdsedDataBlock & ((1 << 30) - 1);
			pcd.fc_A_fCompressed = (fcCompresdsedDataBlock >> 30) & 0x1;
			pcd.fc_B_r1 = (fcCompresdsedDataBlock >> 31) & 0x1;


			pcd.prm_A_fComplex = prmDataBlock & 0x1;
			pcd.prm_data = prmDataBlock & ~0x1;

			data.charactersOffsets.push_back(pcd);
		}
	}

	void readCharacters(_TextData& data, std::istream& document, std::wstringstream& output)
	{
		for (uint32_t i = 0, size = data.charactersPositions.size() - 1; i < size; i++)
		{
			uint32_t symbolsAmount = data.charactersPositions[i + 1] - data.charactersPositions[i];

			if (data.charactersOffsets[i].fc_A_fCompressed == 0)
			{
				std::u16string str(symbolsAmount, 0);
				document.seekg(data.charactersOffsets[i].fc, document.beg);
				document.read((char*)str.data(), sizeof(char16_t) * symbolsAmount);

				std::wstring_convert<std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>> conversion;
				output << conversion.from_bytes(
					reinterpret_cast<const char*>(str.data()),
					reinterpret_cast<const char*>(str.data() + str.size()));
				int a = 0; a++;
			}
			else
			{
				std::string str(symbolsAmount, 0);
				document.seekg(data.charactersOffsets[i].fc / 2, document.beg);
				document.read((char*)str.data(), symbolsAmount);
				output << std::wstring(str.begin(), str.end());
			}
		}
	}

	/* ///////////////////////////////////////////////////

	Read sctions

	/////////////////////////////////////////////////// */

	std::vector<uint32_t> WordBinaryDocumentReader::sections()
	{
		uint32_t sectionsCount = calcPlcElementsNumber(_fibEnd->lcbPlcfSed, 12);

		std::vector<uint32_t> result{};
		result.reserve(sectionsCount);

		_tableStream->base().seekg(_fibEnd->fcPlcfSed, _tableStream->base().beg);

		for (uint32_t i = 0; i < sectionsCount; ++i) {
			result.push_back(_tableStream->getData<uint32_t>());
		}

		return result;
	}


	/* ///////////////////////////////////////////////////

	Other

	/////////////////////////////////////////////////// */


	size_t calcPlcElementsNumber( uint32_t structureSize
								, uint32_t elementSize )
	{
		return (structureSize - 4) / (4 + elementSize);
	}

}
