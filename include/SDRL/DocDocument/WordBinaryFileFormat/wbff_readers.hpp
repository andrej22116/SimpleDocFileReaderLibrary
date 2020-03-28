/** @file wbff_readers.hpp
 *  @brief Provides overrided operators for read WBFF structures
 *  from CPP streams.
 */
#pragma once

#ifndef WBFF_READERS_HPP
#define WBFF_READERS_HPP

#include <istream>
#include "wbff_structures.h"

namespace sdrl {

template<typename CT, typename T>
void read( std::basic_istream<CT>& istream
		 , T& target ) {
	istream.read(reinterpret_cast<CT*>(&target), sizeof(T));
}


template<typename CT = char>
std::basic_istream<CT>& operator >> ( std::basic_istream<CT>& istream
									, FIB_Base& fib )
{
	read(istream, fib.fileIdentifier);
	read(istream, fib.fileFormatVersion);

	// Ignore "unused"
	istream.ignore(sizeof(uint16_t));

	read(istream, fib.languageID);
	read(istream, fib.pnNext);

	uint16_t first_flags = 0;
	read(istream, first_flags);

	// Ignore "nFibBack"
	istream.ignore(sizeof(uint16_t));

	read(istream, fib.lKey);

	// Ignore "envr"
	istream.ignore(sizeof(uint8_t));

	uint8_t second_flags = 0;
	read(istream, second_flags);

	// Ignore reserved fields
	istream.ignore(sizeof(uint16_t) * 2);
	istream.ignore(sizeof(uint32_t) * 2);


	fib.isTemplateDocument = first_flags & 0x1;
	fib.hasOnlyAutotext = (first_flags >> 1) & 0x1;
	fib.lastSaveIsIncremental = (first_flags >> 2) & 0x1;
	fib.hasPictures = (first_flags >> 3) & 0x1;
	fib.quickSavesCount = (first_flags >> 4) & 0xF;
	fib.isEncrypted = (first_flags >> 8) & 0x1;
	fib.hasStreamTableOne = (first_flags >> 9) & 0x1;
	fib.readOnlyRecomended = (first_flags >> 10) & 0x1;
	fib.hasPassword = (first_flags >> 11) & 0x1;
	fib.needOverrideInfo = (first_flags >> 13) & 0x1;
	fib.fromFarEast = (first_flags >> 14) & 0x1;
	fib.useXorObfuscation = (first_flags >> 15) & 0x1;

	fib.needLoadOverridePage = (second_flags >> 2) & 0x1;

	return istream;
}


template<typename CT = char>
std::basic_istream<CT>& operator >> ( std::basic_istream<CT>& istream
									, FIB_RgW97& fib )
{
	istream.ignore(sizeof(uint16_t) * 13);
	read(istream, fib.lidFE);

	return istream;
}


template<typename CT = char>
std::basic_istream<CT>& operator >> ( std::basic_istream<CT>& istream
									, FIB_RgLw97& fib )
{
	read(istream, fib.cbMac);

	istream.ignore(sizeof(uint32_t) * 2);

	read(istream, fib.ccpText);
	read(istream, fib.ccpFtn);
	read(istream, fib.ccpHdd);

	istream.ignore(sizeof(uint32_t));

	read(istream, fib.ccpAtn);
	read(istream, fib.ccpEdn);
	read(istream, fib.ccpTxbx);
	read(istream, fib.ccpHdrTxbx);

	istream.ignore(sizeof(uint32_t) * 11);

	return istream;
}


template<typename CT = char>
std::basic_istream<CT>& operator >> ( std::basic_istream<CT>& istream
									, FIB_RgFcLcb97& fib )
{
	read(istream, fib);
	return istream;
}

template<typename CT = char>
std::basic_istream<CT>& operator >> ( std::basic_istream<CT>& istream
									, FIB_RgFcLcb2000& fib )
{
	read(istream, fib);
	return istream;
}

template<typename CT = char>
std::basic_istream<CT>& operator >> ( std::basic_istream<CT>& istream
									, FIB_RgFcLcb2002& fib )
{
	read(istream, fib);
	return istream;
}

template<typename CT = char>
std::basic_istream<CT>& operator >> ( std::basic_istream<CT>& istream
									, FIB_RgFcLcb2003& fib )
{
	read(istream, fib);
	return istream;
}

template<typename CT = char>
std::basic_istream<CT>& operator >> ( std::basic_istream<CT>& istream
									, FIB_RgFcLcb2007& fib )
{
	read(istream, fib);
	return istream;
}

template<typename CT = char>
std::basic_istream<CT>& operator >> ( std::basic_istream<CT>& istream
									, FIB_RgCswNewData2000& fib )
{
	read(istream, fib);
	return istream;
}

template<typename CT = char>
std::basic_istream<CT>& operator >> ( std::basic_istream<CT>& istream
									, FIB_RgCswNewData2007& fib )
{
	read(istream, fib);
	return istream;
}

}

#endif // WBFF_READERS_HPP
