#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include <fstream>
#include <ctime>

#include "ClassBinaryStreamWrapper/binarystreamwrapper.hpp"
#include "ClassWindowsCompoundBinaryFileFormatReader/windowscompoundbinaryfileformatreader.h"

#include "WindowsCompoundBinaryFileformat/wcbff_structures.h"
#include "variablevisualize.hpp"

#define WINDOWS_TICK 10000000
#define SEC_TO_UNIX_EPOCH 11644473600LL

using namespace std;

template<typename T>
void printDataTypeSize()
{
    cout << sizeof(T) << endl;
}

string convert_UTF16_To_UTF8(u16string stringUTF16);
time_t fileTimeToUnix(WCBFF_FileTime& filetime);
string convert_FileTime_To_UTF8(WCBFF_FileTime& fileTime);

template<typename T>
void showData(char* msg, T variable)
{
    std::cout << msg << variable << " - " << toHex(variable) << std::endl;
}

template<typename T>
void showDataInTableLine(char* msg, T variable)
{
    std::string message = msg;
    std::string decVariable = std::to_string(variable);
    std::string hexVariable = toHex(variable);
    char spacesForHex[] = "                       ";
    char spacesForDec[] = "                    ";

    spacesForHex[23 - hexVariable.size()] = '\0';
    hexVariable += spacesForHex;

    spacesForDec[20 - decVariable.size()] = '\0';
    decVariable += spacesForDec;

    if (message.size() > 34)
    {
        message.resize(31);
        message += "...";
    }

    std::cout << hexVariable << "|" << decVariable << "|" << message << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::wcout.imbue(std::locale(""));

    cout << "Hello World!" << endl;
    {
        ifstream fin("TestDoc.doc", ios::binary);
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

        for (int i = 0; i < 7; i++)
        {
            auto dir = fBinStream.getData<WCBFF_DirectoryEntry>(((header.fatDirSectorAddres + 1) << header.sectorShift) + sizeof(WCBFF_DirectoryEntry) * i);
            std::cout << "Element name: " << convert_UTF16_To_UTF8(u16string((char16_t*)dir.elementName)) << std::endl;
            showDataInTableLine("Object type", dir.objectType);
            showDataInTableLine("Color on tree", dir.colorOnTree);
            showDataInTableLine("Left sibling sid", dir.leftSiblingSid);
            showDataInTableLine("Right sibling sid", dir.rightSiblingSid);
            showDataInTableLine("Child sid", dir.childSid);
            showDataInTableLine("Create time", dir.createTime.lowDateTime);
            showDataInTableLine("Modify time", dir.modifyTime.highDateTime);
            showDataInTableLine("Sector start stream", dir.sectorStartStream);
            showDataInTableLine("Stream size", dir.streamSize);
            std::cout << "Create time: " << convert_FileTime_To_UTF8(dir.createTime) << std::endl;
            std::cout << "Modify time: " << convert_FileTime_To_UTF8(dir.modifyTime) << std::endl;
            std::cout << std::endl;
        }

    }
    /*
    {
        ifstream fin("TestDoc.doc", ios::binary);
        WindowsCompoundBinaryFileFormatReader lol(fin);
    }
    {
        //ifstream fin("MakeFile", ios::binary);
        //WindowsCompoundBinaryFileFormatReader lol(fin);
    }
    */

    return 0;
}

string convert_UTF16_To_UTF8(u16string stringUTF16)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> conversion;
    return conversion.to_bytes(stringUTF16);
}

time_t fileTimeToUnix(WCBFF_FileTime& filetime) {
    long long ll_filetime = filetime.lowDateTime + (((long long)filetime.highDateTime) << 32);
    return (time_t)(ll_filetime / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
}

string convert_FileTime_To_UTF8(WCBFF_FileTime& fileTime)
{
    time_t thisTime = fileTimeToUnix(fileTime);
    return std::string(ctime(&thisTime));
}
