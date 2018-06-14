#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include "ClassBinaryStreamWrapper/binarystreamwrapper.hpp"

using namespace std;

template<typename T>
void printDataTypeSize()
{
    cout << sizeof(T) << endl;
}

string convert_UTF16_To_UTF8(u16string stringUTF16);

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::wcout.imbue(std::locale(""));

    cout << "Hello World!" << endl;
    printDataTypeSize<char>();
    printDataTypeSize<short>();
    printDataTypeSize<int>();
    printDataTypeSize<long long>();
    printDataTypeSize<float>();
    printDataTypeSize<double>();
    {
        ofstream fout("kek", ios::binary);
        char varChar = 's';
        short varShort = 228;
        int varInt = 1488;
        long long varLong = 9999999;
        fout.write((char*)&varChar, sizeof(char));
        fout.write((char*)&varShort, sizeof(short));
        fout.write((char*)&varInt, sizeof(int));
        fout.write((char*)&varLong, sizeof(long long));
    }
    {
        ifstream fin("kek", ios::binary);
        BinaryStreamWrapper finBin(fin);
        long long varLong = finBin.peekData<long long>(sizeof(char) + sizeof(short) + sizeof(int));
        char varChar = finBin.peekData<char>();
        short varShort = finBin.getData<short>(sizeof(char));
        int varInt = finBin.getData<int>();
        wcout << L"\nResult ⬣ ⬢ ⬟ ⭓ "
             << L"\nstream size: " << finBin.getStreamSize()
             << L"\nchar: " << varChar
             << L"\nshort: " << varShort
             << L"\nint: " << varInt
             << L"\nlong: " << varLong << endl;
        cout << convert_UTF16_To_UTF8(u16string(u"\u4f60\u597d⬣ ⬢ ⬟ ⭓ ")) << endl;
    }

    return 0;
}

string convert_UTF16_To_UTF8(u16string stringUTF16)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> conversion;
    return conversion.to_bytes(stringUTF16);
}
