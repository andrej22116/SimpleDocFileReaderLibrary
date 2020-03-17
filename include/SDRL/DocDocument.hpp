#pragma once

#ifndef DOC_DOCUMENT_HPP
#define DOC_DOCUMENT_HPP

#include "IDocument.hpp"

namespace sdrl {
	class DocDocument : public IDocument {
	private:
		std::wstring _text;

	public:
		explicit DocDocument();
		explicit DocDocument(const std::wstring&);

		DocDocument(const DocDocument&);
		DocDocument(DocDocument&&);

		DocDocument& operator = (const DocDocument&);
		DocDocument& operator = (DocDocument&&);

	public:
		virtual void open(const std::wstring& path) override;
		virtual bool isOpened() override;

	public:
		virtual Text allText() override;

		virtual const ParagraphList& paragraphList() override;

		virtual const TableInfo& tableInfo(size_t index) override;
		virtual const TableInfoList& tableInfoList() override;

		virtual const ImageInfo& imageInfo(size_t index) override;
		virtual const ImageInfoList& imageInfoList() override;

		virtual const SectionInfo& sectionInfo(size_t index) override;
		virtual const SectionInfoList& sectionInfoList() override;
	};
}

#endif