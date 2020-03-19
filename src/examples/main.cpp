#include <iostream>
#include <fstream>
#include <ctime>

#include <SDRL.h>

#include <locale>
#include <codecvt>
#include <string>

using namespace std;

int main(int argc, const char** argv)
{
    std::ios_base::sync_with_stdio(false);
    //std::wcout.imbue(std::locale("RU"));
	wcout.imbue(locale("rus_rus.866"));

	std::wcout << "Test: " << std::locale("").name().c_str() << '\n';

	if (argc < 2) {
		std::clog << "Try this: ./programm_name \"path to test file\"" << std::endl;
		return 0;
	}

	sdrl::DocDocument document;
	document.open(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{}.from_bytes(argv[1]));

	auto sections = document.sectionInfoList();
	size_t sectionCount = sections.size();
	size_t currentSection = 0;
	size_t currentParagraph = 0;
	for (const auto& paragraph : document.paragraphList()) {
		if ( sectionCount > currentSection 
			&& sections[currentSection].firstParagraphIndex == currentParagraph ) {
			std::wcout << std::endl << "Section " << ++currentSection << std::endl;
		}
		std::wcout << paragraph << std::endl;
		++currentParagraph;
	}

	std::cin.get();

    return 0;
}
