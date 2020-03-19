#pragma once

#ifndef I_DOCUMENT_HPP
#define I_DOCUMENT_HPP

#include <string>
#include <vector>
#include "DocumentInfo.hpp"

namespace sdrl {
	using Text = std::wstring;
	using Paragraph = std::wstring;
	using ParagraphList = std::vector<std::wstring>;
	using TableInfoList = std::vector<TableInfo>;
	using ImageInfoList = std::vector<ImageInfo>;
	using SectionInfoList = std::vector<SectionInfo>;
	

	class IDocument {
	protected:
		DocumentInfo _documentInfo;
		std::wstring _errMsg;
		bool _isOpened = false;

	public:
		virtual void open(const std::wstring& path) = 0;
		virtual bool isOpened() const = 0;

	public:
		virtual Text allText() const = 0;

		virtual const ParagraphList& paragraphList() const = 0;

		virtual const TableInfo& tableInfo(size_t index) const = 0;
		virtual const TableInfoList& tableInfoList() const = 0;

		virtual const ImageInfo& imageInfo(size_t index) const = 0;
		virtual const ImageInfoList& imageInfoList() const = 0;

		virtual const SectionInfo& sectionInfo(size_t index) const = 0;
		virtual const SectionInfoList& sectionInfoList() const = 0;

	public:
		const DocumentInfo& documentInfo() const { return _documentInfo; }
		const std::wstring& errorText() const { return _errMsg; }

		operator bool() {
			return isOpened();
		}

		virtual ~IDocument() = default;
	};

}

#endif