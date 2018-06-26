#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

#include "ClassBinaryStreamWrapper/binarystreamwrapper.hpp"
#include "ClassWindowsCompoundBinaryFileFormatReader/windowscompoundbinaryfileformatreader.h"

#include "WindowsCompoundBinaryFileformat/wcbff_structures.h"
#include "variablevisualize.hpp"

using namespace std;

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::wcout.imbue(std::locale(""));

    std::string docName = "TestDoc.doc";
/*
    ifstream fin(docName, ios::binary);
    BinaryStreamWrapper fBinStream(fin);
    auto header = fBinStream.peekData<WCBFF_FileHeader>();
    showDataInTableLine("Header size", sizeof(header));
    showDataInTableLine("File signature", header.signature);
    showDataInTableLine("Format minor version", header.minorFormatVersion);
    showDataInTableLine("Format major version", header.majorDllFormatVersion);
    showDataInTableLine("File bytes ordering", header.bytesOrdering);
    showDataInTableLine("FAT sector shift", header.sectorShift);
    showDataInTableLine("FAT mini sector shift", header.miniSectorShift);
    showDataInTableLine("FAT sectors amount", header.fatSectorsAmount);
    showDataInTableLine("FAT dir sector addres", header.fatDirSectorAddres);
    showDataInTableLine("FAT mini sector cutoff", header.miniSectorCutoff);
    showDataInTableLine("Mini FAT first sector addres", header.miniFatChainsFirstSectorAddres);
    showDataInTableLine("Mini FAT sectors amount", header.miniFatSectorsAmount);
    showDataInTableLine("DIF first sector addres", header.difFirstSectorAddres);
    showDataInTableLine("DIF sectors amount", header.difSectorsAmount);
    std::cout << std::endl;
    */
    WindowsCompoundBinaryFileFormatReader lol(docName);
    /*
    {
        //ifstream fin("MakeFile", ios::binary);
        //WindowsCompoundBinaryFileFormatReader lol(fin);
    }
    */

    return 0;
}
