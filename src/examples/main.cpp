#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

#include "ClassWordBinaryFileFormatReader/wordbinaryfileformatreader.h"

using namespace std;

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::wcout.imbue(std::locale(""));
    //std::locale::global(std::locale("en_US.UTF-8"));

    const std::string docName = "TestDoc.doc";
    WordBinaryFileFormatReader lol(docName);

    std::cout << "\nLast save: " << lol.getLastSaveTime_str() << std::endl;

    return 0;
}
