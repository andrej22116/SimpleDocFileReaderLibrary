#pragma once

#ifndef DOCUMENT_INFO_HPP
#define DOCUMENT_INFO_HPP

#include <string>
#include <cinttypes>

namespace sdrl {

	struct DateTime {
		uint32_t datetime = 0;
		uint8_t day() const { return (datetime >> 11) & 0x1F; }
		uint8_t month() const { return (datetime >> 16) & 0xF; }
		uint16_t year() const {	return ((datetime >> 20) & 0x1FF) + 1900; }
		uint8_t hour() const { return (datetime >> 6) & 0x1F; }
		uint8_t minute() const { return datetime & 0x3F; }
		uint8_t weekDay() const { return (datetime >> 29) & 0x7; }

		DateTime() = default;
		DateTime(uint32_t dateTime) { this->datetime = dateTime; }
		DateTime(const DateTime & dateTime) { datetime = dateTime.datetime; }
		DateTime& operator = (uint32_t dateTime) { this->datetime = dateTime; return *this; }
		DateTime& operator = (const DateTime & dateTime) { datetime = dateTime.datetime; return *this; }
	};

	struct DocumentInfo {
		std::wstring name;
		DateTime createTime;
		DateTime lastSave;
		size_t pageCount;
		size_t lineCount;
		size_t wordCount;
		size_t characterCount;
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

} // namespace sdrl

#endif