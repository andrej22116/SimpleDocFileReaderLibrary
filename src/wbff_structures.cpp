#include <sdrl/wbff_structures.hpp>


template<typename T>
void read( std::istream& istream
         , T & target ) {
    istream.read(reinterpret_cast<char*>(&target), sizeof(T));
}


sdrl::FIB_Base::FIB_Base(std::istream& input)
{
    input >> *this;
}


std::istream&
sdrl::operator >> ( std::istream& istream
                  , sdrl::FIB_Base& fib )
{
    read(istream, fib.fileIdentifier);
    read(istream, fib.nFib);

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


    fib.isTemplateDocument    = first_flags & 0x1;
    fib.hasOnlyAutotext       = first_flags & 0x2;
    fib.lastSaveIsIncremental = first_flags & 0x4;
    fib.hasPictures           = first_flags & 0x8;
    fib.quickSavesCount       = first_flags & 0xF0;
    fib.isEncrypted           = first_flags & 0x100;
    fib.hasStreamTableOne     = first_flags & 0x200;
    fib.readOnlyRecomended    = first_flags & 0x400;
    fib.hasPassword           = first_flags & 0x800;
    fib.needOverrideInfo      = first_flags & 0x2000;
    fib.fromFarEast           = first_flags & 0x4000;
    fib.useXorObfuscation     = first_flags & 0x8000;

    fib.needLoadOverridePage  = second_flags & 0x4;
}


std::istream&
sdrl::operator >> ( std::istream& istream
                  , sdrl::FIB_RgW97& fib )
{
    istream.ignore(sizeof(uint16_t) * 13);
    read(istream, fib.lidFE);
}


std::istream&
sdrl::operator >> ( std::istream& istream
                  , sdrl::FIB_RgLw97& fib )
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
}


std::istream&
sdrl::operator >> ( std::istream& istream
                  , sdrl::FIB_RgFcLcb97& fib )
{
    read(istream, fib);
}


std::istream&
sdrl::operator >> ( std::istream& istream
                  , sdrl::FIB_RgFcLcb2000& fib )
{
    read(istream, fib);
}


std::istream&
sdrl::operator >> ( std::istream& istream
                  , sdrl::FIB_RgFcLcb2002& fib )
{
    read(istream, fib);
}


std::istream&
sdrl::operator >> ( std::istream& istream
                  , sdrl::FIB_RgFcLcb2003& fib )
{
    read(istream, fib);
}


std::istream&
sdrl::operator >> ( std::istream& istream
                  , sdrl::FIB_RgFcLcb2007& fib)
{
    read(istream, fib);
}


std::istream&
sdrl::operator >>( std::istream& istream
                 , sdrl::FIB_RgCswNewData2000& fib )
{
    read(istream, fib);
}


std::istream&
sdrl::operator >>( std::istream& istream
                 , sdrl::FIB_RgCswNewData2007& fib )
{
    read(istream, fib);
}
