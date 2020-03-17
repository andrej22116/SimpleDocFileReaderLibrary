#pragma once

#ifndef DOCUMENT_INFO_HPP
#define DOCUMENT_INFO_HPP

#include <string>
#include <cinttypes>

struct DocumentInfo {
	std::wstring name;
	uint64_t createTime;
	uint64_t lastSave;
	size_t pageCount;
	size_t imageCount;
	size_t tableCount;
	size_t paragraphCount;
	size_t sectionCount;
};

struct TableInfo {
	size_t location;	/// Before paragraph index + 1 
	size_t rows;
	size_t columns;
	size_t firstParagraphIndex;
	size_t lastParagraphIndex;
};

struct ImageInfo {
	size_t location;	/// Before paragraph index + 1 
	size_t size;
	size_t width;
	size_t height;
};

struct SectionInfo {
	size_t firstParagraphIndex;
	size_t lastParagraphIndex;
	size_t firstImageIndex;
	size_t lastImageIndex;
	size_t firstTableIndex;
	size_t lastTableIndex;
};

#endif