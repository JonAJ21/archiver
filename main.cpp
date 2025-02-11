#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

#include "rle.h"
#include "mtf.h"
#include "bwt.h"
#include "suffix_tree.h"

std::vector<std::byte> to_bytes(const std::string& str) {
    std::vector<std::byte> bytes(str.size());
    std::transform(std::begin(str), std::end(str), std::back_inserter(bytes), [](char c) {
        return std::byte(c);
    });
    return bytes;
}



int main() {

    std::string input_file_path = "../tests/input.txt";
    std::string output_file_encoded = "../tests/output_encoded.txt";
    std::string output_file_decoded = "../tests/output_decoded.txt";
    

    std::ifstream in(input_file_path, std::ios_base::binary);

    if (!in.is_open()) {
        std::cout << "Error: input file could not open" << std::endl;
        return 1;
    }

    in.seekg(0, std::ios::end);
    std::streamsize size = in.tellg();
    in.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(size);

    if (!in.read(reinterpret_cast<char*>(buffer.data()), size)) {
        std::cerr << "Error: read" << std::endl;
    }

    // for (unsigned char ch : buffer) {
    //     std::cout << ch << ' ' << (uint) ch << ' ' << std::bitset<8>(ch) << std::endl;
    // }

    std::vector<unsigned char> encoded = BWT::encode(buffer);
    encoded = MTF::encode(encoded);
    encoded = RLE::encode(encoded);

    std::vector<unsigned char> decoded = RLE::decode(encoded);
    decoded = MTF::decode(decoded);
    decoded = BWT::decode(decoded);



    std::cout << "size: " << buffer.size() << ' ' << encoded.size() <<' ' << decoded.size() << std::endl;




    
        
    std::ofstream out_encoded(output_file_encoded, std::ios_base::trunc | std::ios_base::binary);

    if (!out_encoded.is_open()) {
        std::cout << "Error: output file could not open" << std::endl;
        return 2;
    }

    for (unsigned char ch : encoded) {
        out_encoded << ch;
    }

    std::ofstream out_decoded(output_file_decoded, std::ios_base::trunc | std::ios_base::binary);

    if (!out_decoded.is_open()) {
        std::cout << "Error: output file could not open" << std::endl;
        return 3;
    }

    for (unsigned char ch : decoded) {
        out_decoded << ch;
    }

    in.close();
    out_encoded.close();
    out_decoded.close();


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