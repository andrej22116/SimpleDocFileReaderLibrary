#include <SDRL/DocDocument.hpp>
#include <SDRL/DocDocument/WordBinaryDocumentReader.hpp>

#include <fstream>
#include <regex>
#include <algorithm>

namespace sdrl {

	struct DocDocument::DocDocumentData {
		Text text;
		ParagraphList paragraphList;
		TableInfoList tableInfoList;
		ImageInfoList imageInfoList;
		SectionInfoList sectionInfoList;
	};

	struct _DocDocumentTemporaryData {
		DocDocument::DocDocumentData& data;
		std::unordered_map<uint32_t, size_t> paragraphBeginsMap;
	};

	void makeParagraphList(_DocDocumentTemporaryData& tempData);
	void makeSectionInfoList(_DocDocumentTemporaryData& tempData, std::vector<uint32_t> list);

	Text sdrl::DocDocument::allText() const
	{
		return _data->text;
	}

	const ParagraphList& sdrl::DocDocument::paragraphList() const
	{
		return _data->paragraphList;
	}

	const TableInfo& sdrl::DocDocument::tableInfo(size_t index) const
	{
		return {};
	}

	const TableInfoList& sdrl::DocDocument::tableInfoList() const
	{
		return {};
	}

	const ImageInfo& sdrl::DocDocument::imageInfo(size_t index) const
	{
		return {};
	}

	const ImageInfoList& sdrl::DocDocument::imageInfoList() const
	{
		return {};
	}

	const SectionInfo& sdrl::DocDocument::sectionInfo(size_t index) const
	{
		return {};
	}

	const SectionInfoList& sdrl::DocDocument::sectionInfoList() const
	{
		return {};
	}


	DocDocument::DocDocument() : 
		_data(new DocDocumentData)
	{
	}

	DocDocument::DocDocument(const std::wstring&) : 
		_data(new DocDocumentData)
	{
	}

	DocDocument::DocDocument(const DocDocument&) : 
		_data(new DocDocumentData)
	{
	}

	DocDocument::DocDocument(DocDocument&&) : _data(nullptr)
	{
	}

	DocDocument& DocDocument::operator=(const DocDocument&)
	{
		return *this;
	}

	DocDocument& DocDocument::operator=(DocDocument&&)
	{
		return *this;
	}

	void sdrl::DocDocument::open(const std::wstring& path)
	{
		std::ifstream stream{ path, std::ios::binary };
		if (!stream) {
			_errMsg = L"File \"" + path + L"\" can't be opedned!";
			return;
		}

		WordBinaryDocumentReader reader{ stream, 0 };
		_data->text = reader.text();

		_DocDocumentTemporaryData tempData{
			*_data
		};
		makeParagraphList(tempData);
		makeSectionInfoList(tempData, reader.sections());

		stream.close();

		_isOpened = true;
	}

	bool sdrl::DocDocument::isOpened() const
	{
		return false;
	}


	void makeParagraphList(_DocDocumentTemporaryData& tempData)
	{
		std::wregex regex{ L"((.+?)(?:\r|\f|\a)+)" };
		std::wsregex_token_iterator regexIterator{ tempData.data.text.begin(), tempData.data.text.end(), regex, 2 };
		std::wsregex_token_iterator regexEnd{};

		auto currentIter = tempData.data.text.cbegin();
		size_t currentTextIndex = 0;

		for (; regexIterator != regexEnd; ++regexIterator) {
			currentTextIndex += std::distance(currentIter, regexIterator->first);
			currentIter = regexIterator->first;
			tempData.paragraphBeginsMap[currentTextIndex] = tempData.data.paragraphList.size();
			tempData.data.paragraphList.emplace_back(std::move(*regexIterator));
		}
	}

	void makeSectionInfoList(_DocDocumentTemporaryData& tempData, std::vector<uint32_t> list)
	{
		tempData.data.sectionInfoList.reserve(list.size() - 1);

		for (int i = 0, length = list.size() - 1; i < length; ++i) {
			tempData.data.sectionInfoList.push_back({
				tempData.paragraphBeginsMap[list[i]],
				tempData.paragraphBeginsMap[list[i + 1]] - 1
			});
		}

		tempData.data.sectionInfoList[list.size() - 2].lastParagraphIndex = tempData.data.paragraphList.size() - 1;
	}
}