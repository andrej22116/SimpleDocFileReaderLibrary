/** @file wbff_readers.hpp
 *  @brief Provides overrided operators for read WBFF structures
 *  from CPP streams.
 */

#ifndef WBFF_READERS_HPP
#define WBFF_READERS_HPP

#include <istream>

namespace sdrl {

struct FIB_Base;
/// overloaded operator for read data from input stream
std::istream& operator >> ( std::istream& istream
                          , FIB_Base& fib );

struct FIB_RgW97;
/// overloaded operator for read data from input stream
std::istream& operator >> ( std::istream& istream
                          , FIB_RgW97& fib );

struct FIB_RgLw97;
/// overloaded operator for read data from input stream
std::istream& operator >> ( std::istream& istream
                          , FIB_RgLw97& fib );

struct FIB_RgFcLcb97;
/// overloaded operator for read data from input stream
std::istream& operator >> ( std::istream& istream
                          , FIB_RgFcLcb97& fib );

struct FIB_RgFcLcb2000;
/// overloaded operator for read data from input stream
std::istream& operator >> ( std::istream& istream
                          , FIB_RgFcLcb2000& fib );

struct FIB_RgFcLcb2002;
/// overloaded operator for read data from input stream
std::istream& operator >> ( std::istream& istream
                          , FIB_RgFcLcb2002& fib );

struct FIB_RgFcLcb2003;
/// overloaded operator for read data from input stream
std::istream& operator >> ( std::istream& istream
                          , FIB_RgFcLcb2003& fib );

struct FIB_RgFcLcb2007;
/// overloaded operator for read data from input stream
std::istream& operator >> ( std::istream& istream
                          , FIB_RgFcLcb2007& fib );

struct FIB_RgCswNewData2000;
/// overloaded operator for read data from input stream
std::istream& operator >> ( std::istream& istream
                          , FIB_RgCswNewData2000& fib );

struct FIB_RgCswNewData2007;
/// overloaded operator for read data from input stream
std::istream& operator >> ( std::istream& istream
                          , FIB_RgCswNewData2007& fib );

}

#endif // WBFF_READERS_HPP
