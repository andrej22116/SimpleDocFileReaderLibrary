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

    std::string docName = "TestDoc_3.doc";

    cout << "Hello World!" << endl;
    {
        ifstream fin(docName, ios::binary);
        BinaryStreamWrapper fBinStream(fin);

        auto header = fBinStream.getData<WCBFF_FileHeader>();
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
        showDataInTableLine("Mini FAT first sector addres", header.miniFatFirstSectorAddres);
        showDataInTableLine("Mini FAT sectors amount", header.miniFatSectorsAmount);
        showDataInTableLine("DIF first sector addres", header.difFirstSectorAddres);
        showDataInTableLine("DIF sectors amount", header.difSectorsAmount);
        std::cout << std::endl;
        /*
        for (int i = 0; i < 7; i++)
        {
            auto dir = fBinStream.getData<WCBFF_DirectoryEntry>(((header.fatDirSectorAddres + 1) << header.sectorShift) + sizeof(WCBFF_DirectoryEntry) * i);
            std::string streamName = convert_UTF16_To_UTF8(u16string((char16_t*)dir.elementName));
            std::cout << "Element name: " << streamName << std::endl;
            showDataInTableLine("Struct size", sizeof(WCBFF_DirectoryEntry));
            showDataInTableLine("Object type", dir.objectType);
            showDataInTableLine("Color on tree", dir.colorOnTree);
            showDataInTableLine("Left sibling sid", dir.leftSiblingSid);
            showDataInTableLine("Right sibling sid", dir.rightSiblingSid);
            showDataInTableLine("Child sid", dir.childSid);
            showDataInTableLine("Create time low", dir.createTime.lowDateTime);
            showDataInTableLine("Create time high", dir.createTime.highDateTime);
            showDataInTableLine("Modify time low", dir.modifyTime.lowDateTime);
            showDataInTableLine("Modify time high", dir.modifyTime.highDateTime);
            showDataInTableLine("Sector start stream", dir.sectorStartStream);
            showDataInTableLine("Stream size", dir.streamSize);
            std::cout << "Create time: " << convert_FileTime_To_UTF8(dir.createTime) << std::endl;
            std::cout << "Modify time: " << convert_FileTime_To_UTF8(dir.modifyTime) << std::endl;
            std::cout << std::endl;
        }
        */
        fin.close();

    }

    {
        ifstream fin(docName, ios::binary);
        WindowsCompoundBinaryFileFormatReader lol(fin);
    }
    /*
    {
        //ifstream fin("MakeFile", ios::binary);
        //WindowsCompoundBinaryFileFormatReader lol(fin);
    }
    */

    return 0;
}
