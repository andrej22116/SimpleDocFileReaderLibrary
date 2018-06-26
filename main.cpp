#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

#include "ClassWindowsCompoundBinaryFileFormatReader/windowscompoundbinaryfileformatreader.h"

using namespace std;

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::wcout.imbue(std::locale(""));

    std::string docName = "TestDoc.doc";
    WindowsCompoundBinaryFileFormatReader lol(docName);

    return 0;
}
