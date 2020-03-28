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

	const auto& docInfo = document.documentInfo();
	std::wcout << std::endl
		<< "Document info:" << std::endl

		<< "Page count: " << docInfo.pageCount << std::endl
		<< "Word count: " << docInfo.wordCount << std::endl
		<< "Character count: " << docInfo.characterCount << std::endl

		<< "Create time (dd.mm.yyyy hh:mm): " 
		<< docInfo.createTime.day() << "."
		<< docInfo.createTime.month() << "."
		<< docInfo.createTime.year() << " "
		<< docInfo.createTime.hour() << ":" 
		<< docInfo.createTime.minute() << std::endl

		<< "Last save time (dd.mm.yyyy hh:mm): " 
		<< docInfo.lastSave.day() << "."
		<< docInfo.lastSave.month() << "."
		<< docInfo.lastSave.year() << " "
		<< docInfo.lastSave.hour() << ":" 
		<< docInfo.lastSave.minute() << std::endl;

	std::cin.get();

    return 0;
}
