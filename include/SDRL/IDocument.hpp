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
		virtual bool isOpened() = 0;

	public:
		virtual Text allText() = 0;

		virtual const ParagraphList& paragraphList() = 0;

		virtual const TableInfo& tableInfo(size_t index) = 0;
		virtual const TableInfoList& tableInfoList() = 0;

		virtual const ImageInfo& imageInfo(size_t index) = 0;
		virtual const ImageInfoList& imageInfoList() = 0;

		virtual const SectionInfo& sectionInfo(size_t index) = 0;
		virtual const SectionInfoList& sectionInfoList() = 0;

	public:
		const DocumentInfo& documentInfo() { return _documentInfo; }
		const std::wstring& errorText() { return _errMsg; }

		operator bool() {
			return isOpened();
		}

		virtual ~IDocument() = default;
	};

}

#endif