#include <SDRL/DocDocument/WordBinaryDocumentReader.hpp>
#include <SDRL/DocDocument/variablevisualize.hpp>
#include <SDRL/DocDocument/WordBinaryFileFormat/wbff_structures.h>
#include <SDRL/DocDocument/WordBinaryFileFormat/wbff_readers.hpp>

namespace sdrl {

	struct WordBinaryDocumentReader::FIB_Begin
	{
		FIB_Base fibBase;
		FIB_RgW97 fibRgW97;
		FIB_RgLw97 fibLw97;
	};


	struct WordBinaryDocumentReader::CLX_Data {
		std::vector<uint32_t> characterSequences;
		std::vector<Pcd> characterSequencesOffsets;

		int16_t prlArraySize;

		uint32_t plcPcdSize;
		uint32_t lastCpValue;
	};



	WordBinaryDocumentReader::WordBinaryDocumentReader(const std::wstring& fileName, uint16_t flags)
		: WindowsCompoundBinaryFileFormatReader(fileName), _flags(flags)
	{
		readDocument();
	}

	WordBinaryDocumentReader::WordBinaryDocumentReader(std::istream& stream, uint16_t flags)
		: WindowsCompoundBinaryFileFormatReader(stream), _flags(flags)
	{
		readDocument();
	}


	void WordBinaryDocumentReader::readDocument()
	{
		_wordDocumentStream = std::make_shared<InputBinaryStream<char>>(getStream("WordDocument"));

		_fibBegin = std::make_shared<FIB_Begin>();

		_wordDocumentStream->base() >> _fibBegin->fibBase;
		_wordDocumentStream->base().ignore(sizeof(uint16_t));
		_wordDocumentStream->base() >> _fibBegin->fibRgW97;
		_wordDocumentStream->base().ignore(sizeof(uint16_t));
		_wordDocumentStream->base() >> _fibBegin->fibLw97;
		_wordDocumentStream->base().ignore(sizeof(uint16_t));

		std::string nameofTableStream("0Table");
		if ( _fibBegin->fibBase.hasStreamTableOne )
		{
			nameofTableStream = "1Table";
		}

		_tableStream = std::make_shared<InputBinaryStream<char>>(getStream(nameofTableStream));

		switch (_fibBegin->fibBase.fileFormatVersion)
		{
		case 0x00C1: {
			std::cout << "File FIB version: 1997!\n" << std::endl;
			auto fibEnd = _wordDocumentStream->getData<FIB_RgFcLcb97>();
			readDocument97(fibEnd);
		} break;
		case 0x00D9: {
			std::cout << "File FIB version: 2000!\n" << std::endl;
			auto fibEnd = _wordDocumentStream->getData<FIB_RgFcLcb2000>();
			readDocument00(fibEnd);
		} break;
		case 0x0101: {
			std::cout << "File FIB version: 2002!\n" << std::endl;
			auto fibEnd = _wordDocumentStream->getData<FIB_RgFcLcb2002>();
			readDocument02(fibEnd);
		} break;
		case 0x010C: {
			std::cout << "File FIB version: 2003!\n" << std::endl;
			auto fibEnd = _wordDocumentStream->getData<FIB_RgFcLcb2003>();
			readDocument03(fibEnd);
		} break;
		case 0x0112: {
			std::cout << "File FIB version: 2007!\n" << std::endl;
			auto fibEnd = _wordDocumentStream->getData<FIB_RgFcLcb2007>();
			readDocument07(fibEnd);
		} break;
		}

		clearData();
		_wordDocumentStream.reset();
		_tableStream.reset();
		_fibBegin.reset();
	}


	void WordBinaryDocumentReader::readDocument97(FIB_RgFcLcb97& fibEnd)
	{
		// Read date for document 1997
		std::cout << "Read version: 1997!" << std::endl;

		//readCpDiapasons(fibEnd);

		readClxArray(fibEnd);
		readCharacters();

		readDateTime(fibEnd);

		std::cout << std::endl;
	}


	void WordBinaryDocumentReader::readDocument00(FIB_RgFcLcb2000& fibEnd)
	{
		// Read date for document 2000
		readDocument97(fibEnd);
		std::cout << "Read version: 2000!" << std::endl;
	}


	void WordBinaryDocumentReader::readDocument02(FIB_RgFcLcb2002& fibEnd)
	{
		// Read date for document 2002
		readDocument00(fibEnd);
		std::cout << "Read version: 2002!" << std::endl;
	}


	void WordBinaryDocumentReader::readDocument03(FIB_RgFcLcb2003& fibEnd)
	{
		// Read date for document 2003
		readDocument02(fibEnd);
		std::cout << "Read version: 2003!" << std::endl;
	}


	void WordBinaryDocumentReader::readDocument07(FIB_RgFcLcb2007& fibEnd)
	{
		// Read date for document 2007. No data. :3
		readDocument03(fibEnd);
		std::cout << "Read version: 2007!" << std::endl;
	}



	void WordBinaryDocumentReader::readClxArray(FIB_RgFcLcb97& fibEnd)
	{
		readPrcArray(fibEnd);
		readPcdtArray();
	}


	void WordBinaryDocumentReader::readPrcArray(FIB_RgFcLcb97& fibEnd)
	{
		_tableStream->base().seekg(fibEnd.fcClx, _tableStream->base().beg);

		while (_tableStream->peekData<uint8_t>() == 0x01)
		{
			_tableStream->getData<uint8_t>();
			readPrcDataArray();
		}
	}


	void WordBinaryDocumentReader::readPrcDataArray()
	{
		auto cbGrpprl = _tableStream->getData<int16_t>();
		readPrlArray(cbGrpprl);
	}


	void WordBinaryDocumentReader::readPrlArray(int16_t prlArraySize)
	{
		static const uint16_t bufferSize = 1024;
		static char buffer[bufferSize];

		// Заглушка
		for (uint16_t needReadBytes = prlArraySize; needReadBytes > 0;)
		{
			uint16_t bytesAmountForRead = needReadBytes < bufferSize ? needReadBytes : bufferSize;
			needReadBytes -= bytesAmountForRead;

			_tableStream->base().read(buffer, bytesAmountForRead);
		}
	}




	void WordBinaryDocumentReader::readPcdtArray()
	{
		while (_tableStream->peekData<uint8_t>() == 0x02)
		{
			_tableStream->getData<uint8_t>();
			readPlcPcdArray(_tableStream->getData<uint32_t>());
		}
	}


	void WordBinaryDocumentReader::readPlcPcdArray(uint32_t plcPcdSize)
	{
		_charactersPositions.reserve(plcPcdSize / sizeof(uint32_t));

		FIB_RgLw97& fibLw = _fibBegin->fibLw97;
		uint32_t lastCP = fibLw.ccpText;
		uint32_t ccpSumm = fibLw.ccpAtn + fibLw.ccpEdn + fibLw.ccpFtn + fibLw.ccpHdd + fibLw.ccpHdrTxbx + fibLw.ccpTxbx;
		if (ccpSumm > 0)
		{
			lastCP += ccpSumm + 1;
		}

		readCpArray(lastCP);
		readPlcArray();
	}


	void WordBinaryDocumentReader::readCpArray(uint32_t lastCpValue)
	{
		uint32_t cpOffset = 0;
		do
		{
			cpOffset = _tableStream->getData<uint32_t>();
			_charactersPositions.push_back(cpOffset);
		} while (cpOffset != lastCpValue);
	}


	void WordBinaryDocumentReader::readPlcArray()
	{
		for (uint32_t i = 1; i < _charactersPositions.size(); i++)
		{
			
			auto firstDataBlock = _tableStream->getData<uint16_t>();
			auto fcCompresdsedDataBlock = _tableStream->getData<uint32_t>();
			auto prmDataBlock = _tableStream->getData<uint16_t>();

			Pcd pcd;
			pcd.A_fNoParaLast = firstDataBlock & 0x1;
			pcd.B_fR1 = firstDataBlock & 0x2;
			pcd.C_fDirty = firstDataBlock & 0x4;

			pcd.fc = fcCompresdsedDataBlock & ((1 << 30) - 1);
			pcd.fc_A_fCompressed = fcCompresdsedDataBlock & (1 << 30);
			pcd.fc_B_r1 = fcCompresdsedDataBlock & (1 << 31);


			pcd.prm_A_fComplex = prmDataBlock & 0x1;
			pcd.prm_data = prmDataBlock & ~0x1;

			_charactersOffsets.push_back(pcd);
		}
	}


	void WordBinaryDocumentReader::readCharacters()
	{
		for (uint32_t i = 0, size = _charactersPositions.size() - 1; i < size; i++)
		{
			uint32_t symbolsAmount = _charactersPositions[i + 1] - _charactersPositions[i];

			if (_charactersOffsets[i].fc_A_fCompressed == 0)
			{
				std::u16string str(symbolsAmount, 0);
				_wordDocumentStream->base().seekg(_charactersOffsets[i].fc, _wordDocumentStream->base().beg);
				_wordDocumentStream->base().read((char*)str.data(), sizeof(char16_t) * symbolsAmount);

				std::wstring_convert<std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>> conversion;
				_characterStream << conversion.from_bytes(
					reinterpret_cast<const char*> (&str[0]),
					reinterpret_cast<const char*> (&str[0] + str.size()));
				int a = 0; a++;
			}
			else
			{
				std::string str(symbolsAmount, 0);
				_wordDocumentStream->base().seekg(_charactersOffsets[i].fc / 2, _wordDocumentStream->base().beg);
				_wordDocumentStream->base().read((char*)str.data(), symbolsAmount);
				_characterStream << std::wstring(str.begin(), str.end());
			}
		}
	}


	void WordBinaryDocumentReader::readDateTime(FIB_RgFcLcb97& fibEnd)
	{
		_timeLastSave = getTime(fibEnd.dwLowDateTime, fibEnd.dwHighDateTime);
		std::tm* tm = std::localtime(&_timeLastSave);
		std::stringstream ss;
		ss << std::put_time(tm, "Date: %F; Time: %T");
		_timeLastSave_str = ss.str();
	}


	time_t WordBinaryDocumentReader::getTime(uint32_t dwLowDateTime, uint32_t dwHighDateTime)
	{
		static const uint64_t EPOCH_DIFFERENCE_MICROS = 11644473600LL;
		uint64_t total_us = ((uint64_t)dwHighDateTime << 32) + dwLowDateTime;
		//total_us -= EPOCH_DIFFERENCE_MICROS;

		return (time_t)(total_us / 10000000 - EPOCH_DIFFERENCE_MICROS);
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


	///////////////////////////////////////////
	/// Reading characters diapasons! Begin ///
	///////////////////////////////////////////

	void WordBinaryDocumentReader::readCpDiapasons(FIB_RgFcLcb97& fibEnd)
	{
		readOffsetsOfDataDiapasons(fibEnd);
		readOffsetsOfSequenceStructuresWithCharactersDiapasons();
		readCharacterDiapasons();
	}


	void WordBinaryDocumentReader::readOffsetsOfDataDiapasons(FIB_RgFcLcb97& fibEnd)
	{
		_tableStream->base().seekg(fibEnd.fcPlcfBtePapx, _wordDocumentStream->base().beg);
		uint32_t offset = 0;
		while (_tableStream->peekData<uint32_t>() > offset)
		{
			offset = _tableStream->getData<uint32_t>();
			_offsetsOfDataDiapasons.push_back(offset);
		}
	}


	void WordBinaryDocumentReader::readOffsetsOfSequenceStructuresWithCharactersDiapasons()
	{
		for (int i = 0, size = _offsetsOfDataDiapasons.size() - 1; i < size; i++)
		{
			uint32_t offset = _tableStream->getData<uint32_t>();
			offset <<= 10;
			offset >>= 10;
			_offsetsOfSequenceStructuresWithCharactersDiapasons.push_back(offset * 512);
		}
	}


	void WordBinaryDocumentReader::readCharacterDiapasons()
	{
		for (auto offset : _offsetsOfSequenceStructuresWithCharactersDiapasons)
		{
			_wordDocumentStream->base().seekg(offset, _wordDocumentStream->base().beg);
			auto rgfcAmount = _wordDocumentStream->peekData<uint8_t>(offset + 511);

			if (_characterDiapasons.empty())
			{
				_characterDiapasons.push_back(_wordDocumentStream->getData<uint32_t>());
			}
			else
			{
				_wordDocumentStream->base().ignore(sizeof(uint32_t));
			}
			for (int i = 0; i < rgfcAmount; i++)
			{
				_characterDiapasons.push_back(_wordDocumentStream->getData<uint32_t>());
			}

			for (int i = 0; i < rgfcAmount; i++)
			{
				_characterDiapasonsPropertiesOffsets.push_back(_wordDocumentStream->getData<uint8_t>());
				_wordDocumentStream->base().ignore(sizeof(uint32_t) * 3);
			}

			for (uint16_t structOffset : _characterDiapasonsPropertiesOffsets)
			{
				if (structOffset == 0) continue;

				structOffset *= 2;
				_wordDocumentStream->base().seekg(offset + structOffset, _wordDocumentStream->base().beg);

				uint16_t structSize = _wordDocumentStream->getData<uint8_t>();
				if (structSize == 0)
				{
					structSize = _wordDocumentStream->getData<uint8_t>() * 2;
				}
				else
				{
					structSize = (structSize * 2) - 1;
				}

				_characterDiapasonsStyles.push_back(_wordDocumentStream->getData<uint16_t>());
				for (int bytesReaded = sizeof(uint16_t); bytesReaded < structSize;)
				{
					Sprm sprm(_wordDocumentStream->getData<uint16_t>());
					_characterDiapasonsModify.push_back(sprm);
					bytesReaded += sizeof(uint16_t);

					switch (sprm.spra)
					{
					case 0: case 1: {
						_characterDiapasonsModifyOperands.push_back(_wordDocumentStream->getData<uint8_t>());
						bytesReaded += sizeof(uint8_t);
					} break;
					case 2: case 4: case 5: {
						_characterDiapasonsModifyOperands.push_back(_wordDocumentStream->getData<uint16_t>());
						bytesReaded += sizeof(uint16_t);
					} break;
					case 3: {
						_characterDiapasonsModifyOperands.push_back(_wordDocumentStream->getData<uint32_t>());
						bytesReaded += sizeof(uint32_t);
					} break;
					case 7: {
						uint32_t operand = 0;
						_wordDocumentStream->base().read((char*)&operand, 3);
						_characterDiapasonsModifyOperands.push_back(operand);
						bytesReaded += 3;
					} break;
					case 6: {
						auto operandSize = _wordDocumentStream->getData<uint8_t>();
						if (operandSize > sizeof(uint32_t))
						{
							std::cout << "Operand with type 6 has size > 4! Size: " << uint32_t(operandSize) << std::endl;
							_characterDiapasonsModifyOperands.push_back(_wordDocumentStream->getData<uint32_t>());
							_wordDocumentStream->base().ignore(operandSize - sizeof(uint32_t));
						}
						else
						{
							uint32_t operand = 0;
							_wordDocumentStream->base().read((char*)&operand, operandSize);
							_characterDiapasonsModifyOperands.push_back(operand);
						}
						bytesReaded += operandSize;
					} break;
					}
				}
			}
		}
	}




	void WordBinaryDocumentReader::makeContainers()
	{
		modifyDiapasonsForWorkWithCharacterStream();
		readCharactersAndCreateContainers();
	}

	void WordBinaryDocumentReader::modifyDiapasonsForWorkWithCharacterStream()
	{
		/*uint32_t characterPartIndex = 0;
		uint32_t subForCorrectingOffset = _charactersOffsets[characterPartIndex].fc.fc;

		uint32_t minOffset = 0;
		uint32_t maxOffset = minOffset
			+ ((_charactersPositions.size() > 1)
				? _charactersPositions[1] - _charactersPositions[0]
				: _charactersPositions[0])
			* 2;

		for (auto& offset : _characterDiapasons)
		{
			uint32_t newOffset = offset - subForCorrectingOffset;
			if (newOffset > maxOffset)
			{
				characterPartIndex++;
				if (maxOffset != (_charactersOffsets[characterPartIndex].fc.fc - subForCorrectingOffset))
				{
					subForCorrectingOffset += _charactersOffsets[characterPartIndex].fc.fc
						- subForCorrectingOffset - maxOffset;
				}
				minOffset = maxOffset;
				maxOffset = minOffset
					+ (_charactersPositions[characterPartIndex + 1]
						- _charactersPositions[characterPartIndex]) * 2;

				newOffset = offset - subForCorrectingOffset;
			}

			offset = newOffset;
		}*/
	}

	void WordBinaryDocumentReader::readCharactersAndCreateContainers()
	{
		/*std::stack<std::shared_ptr<Container>> stackOfParentContainers;
		std::shared_ptr<Container> newElement;

		int counter_paragraph = 0;
		int counter_table = 0;

		_characterStream.seekg(0, _characterStream.beg);
		std::wstring str;
		for (int i = 0, size = _characterDiapasons.size() - 1; i < size; i++)
		{
			uint32_t stringSize = ((_characterDiapasons[i + 1] - _characterDiapasons[i]) / 2) - 1;
			str.resize(stringSize, 0);

			_characterStream.read((wchar_t*)str.data(), stringSize);
			uint32_t bytesReaded = _characterStream.gcount();

			wchar_t lastSymbol;
			_characterStream.read((wchar_t*)&lastSymbol, 1);

			if (bytesReaded > 0)
			{
				newElement = std::make_shared<TextContainer>(str);
			}

			std::wcout << L"Str: " << str << " | last symbol: " << (uint16_t)lastSymbol << std::endl;

			switch (lastSymbol)
			{
			case Mark_Paragraph: {
				counter_table = 0;
				counter_paragraph++;
			} break;
			case Mark_Table: {
				counter_paragraph = 0;
				counter_table++;
			} break;
			case Mark_Image: {} break;
			}
		}*/
	}

}

