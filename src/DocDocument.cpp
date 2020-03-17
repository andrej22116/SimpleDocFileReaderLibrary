#include <SDRL/DocDocument.hpp>
#include <SDRL/DocDocument/WordBinaryDocumentReader.hpp>

#include <fstream>

namespace sdrl {

	Text sdrl::DocDocument::allText()
	{
		return _text;
	}

	const ParagraphList& sdrl::DocDocument::paragraphList()
	{
		return {};
	}

	const TableInfo& sdrl::DocDocument::tableInfo(size_t index)
	{
		return {};
	}

	const TableInfoList& sdrl::DocDocument::tableInfoList()
	{
		return {};
	}

	const ImageInfo& sdrl::DocDocument::imageInfo(size_t index)
	{
		return {};
	}

	const ImageInfoList& sdrl::DocDocument::imageInfoList()
	{
		return {};
	}

	const SectionInfo& sdrl::DocDocument::sectionInfo(size_t index)
	{
		return {};
	}

	const SectionInfoList& sdrl::DocDocument::sectionInfoList()
	{
		return {};
	}

	DocDocument::DocDocument()
	{
	}

	DocDocument::DocDocument(const std::wstring&)
	{
	}

	DocDocument::DocDocument(const DocDocument&)
	{
	}

	DocDocument::DocDocument(DocDocument&&)
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
		_text = reader.text();

		stream.close();

		_isOpened = true;
	}

	bool sdrl::DocDocument::isOpened()
	{
		return false;
	}

}