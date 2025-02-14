#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

#include "rle.h"
#include "mtf.h"
#include "bwt.h"
#include "huffman.h"
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

    in.close();


    // for (unsigned char ch : buffer) {
    //     std::cout << ch << ' ' << (uint) ch << ' ' << std::bitset<8>(ch) << std::endl;
    // }

    std::vector<unsigned char> encoded = BWT::encode(buffer);
    encoded = MTF::encode(encoded);
    encoded = RLE::encode(encoded);
    encoded = Huffman::encode(encoded);

    std::vector<unsigned char> decoded = Huffman::decode(encoded);
    decoded = RLE::decode(decoded);
    decoded = MTF::decode(decoded);
    decoded = BWT::decode(decoded);
 
    std::cout << encoded.size() << std::endl;
    std::cout << decoded.size() << std::endl;
    std::cout << decoded.size() / (double) encoded.size();

    
        
    std::ofstream out_encoded(output_file_encoded, std::ios_base::trunc | std::ios_base::binary);

    if (!out_encoded.is_open()) {
        std::cout << "Error: output file could not open" << std::endl;
        return 2;
    }

    for (unsigned char ch : encoded) {
        out_encoded << ch;
    }

    out_encoded.close();

    std::ofstream out_decoded(output_file_decoded, std::ios_base::trunc | std::ios_base::binary);

    if (!out_decoded.is_open()) {
        std::cout << "Error: output file could not open" << std::endl;
        return 3;
    }

    for (unsigned char ch : decoded) {
        out_decoded << ch;
    }

    out_decoded.close();

    // auto huffman = Huffman::encode(buffer);
    // std::cout << "===============================================\n";
    // for (auto ch : huffman) {
    //     std::cout << std::bitset<8>(ch) << ' ';
    // }
    // std::cout << std::endl;
    // std::cout << "===============================================\n";
    // auto decoded_huffman = Huffman::decode(huffman);

    // for (auto ch : decoded_huffman) {
    //     std::cout << ch;
    // }
    // std::cout << std::endl;
    // std::cout << decoded_huffman.size() << std::endl;
    

    return 0;
}