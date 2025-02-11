#include <iostream>
#include <bitset>


#include "rle.h"
#include "mtf.h"
#include "bwt.h"
#include "suffix_tree.h"

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    
    std::string input;
    std::string str;
    //std::getline(std::wcin, str);
    while (std::getline(std::cin, str)) {
        input += str + '\n';
    }
    input.pop_back();
    
    input += (unsigned char) 255;

    std::cout << input << std::endl << std::endl;
    
    for (unsigned char ch : input) {
        std::cout << (int) ch << ' ' << ch << std::endl;
    }
    std::cout << "=================" << std::endl;
    wchar_t c = L'б';
    std::wcout << (size_t) c << ' ' << c << ' ' << std::bitset<16> (c)<< std::endl;

    
    //std::cout << sizeof(std::byte) << std::endl;
    // std::string bitsStr = "11111111111";
    // std::bitset<1> bits(bitsStr);
    // std::cout << bits << std::endl;
    // std::cout << bits.to_ulong() << std::endl;



    // setlocale(LC_ALL, "");
    // std::wstring command;
    // std::wcin >> command;
    // if (command == L"compress") {
    //     std::wstring input;

    //     std::wstring str;
    //     // while (std::wcin >> str) {
    //     //     input += str;
    //     // }
    //     std::getline(std::wcin, str);
    //     while (std::getline(std::wcin, str)) {
    //         input += str + L'\n';
    //     }
    //     input.pop_back();

    //     if (input.size() == 0) {
    //         return 0;
    //     }

    //     // std::wcout << input << std::endl;
        
    
    //     std::wstring bwtOutput = BWT::encode(input);
    //     std::wstring mtfOutput = MTF::encode(bwtOutput);
    //     std::wstring rleOutput = RLE::encode(mtfOutput);

    //     std::wcout << rleOutput << std::endl;

    //     std::wcout << "================================================================" << std::endl;

    //     rleOutput = RLE::decode(rleOutput);
    //     mtfOutput = MTF::decode(rleOutput);
    //     bwtOutput = BWT::decode(mtfOutput);
        
    //     std::wcout << bwtOutput << std::endl;

    //     std::wcout << "================================================================" << std::endl;
        
    //     std::wcout << input << std::endl;
        

    // } else if (command == L"decompress") {
    //     std::wstring input;
    //     std::wstring str;
    //     while (std::wcin >> str) {
    //         input += str + L'\n';
    //     }        

    //     // std::string rleOutput = RLE::decode(input);
    //     // std::string mtfOutput = MTF::decode(rleOutput);
    //     // std::string bwtOutput = BWT::decode(mtfOutput);
        
    //     // std::cout << bwtOutput << std::endl;
    // }

    return 0;
}