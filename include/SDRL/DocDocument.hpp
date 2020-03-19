#pragma once

#ifndef DOC_DOCUMENT_HPP
#define DOC_DOCUMENT_HPP

#include "IDocument.hpp"
#include <memory>

namespace sdrl {
	class DocDocument : public IDocument {
	public:
		explicit DocDocument();
		explicit DocDocument(const std::wstring&);

		DocDocument(const DocDocument&);
		DocDocument(DocDocument&&);

		DocDocument& operator = (const DocDocument&);
		DocDocument& operator = (DocDocument&&);

	public:
		virtual void open(const std::wstring& path) override;
		virtual bool isOpened() const override;

	public:
		virtual Text allText() const override;

		virtual const ParagraphList& paragraphList() const override;

		virtual const TableInfo& tableInfo(size_t index) const override;
		virtual const TableInfoList& tableInfoList() const override;

		virtual const ImageInfo& imageInfo(size_t index) const override;
		virtual const ImageInfoList& imageInfoList() const override;

		virtual const SectionInfo& sectionInfo(size_t index) const override;
		virtual const SectionInfoList& sectionInfoList() const override;

	
	public:
		struct DocDocumentData;
	private:
		std::shared_ptr<DocDocumentData> _data;
	};
}

#endif