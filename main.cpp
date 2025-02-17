#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

#include "rle.h"
#include "mtf.h"
#include "bwt.h"
#include "huffman.h"
#include "suffix_tree.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cout << "There must be 3 arguments:\n 1) compress/decompress\n 2) path to input file\n 3) path to output file\n";
    }

    std::string command = argv[1];
    std::string inputFilePath = argv[2];
    std::string outputFilePath = argv[3];
    
    bool compress;
    if (command == "compress") {
        compress = true;
    } else if (command == "decompress") {
        compress = false;
    } else {
        std::cout << "Error: invalid command" << std::endl;
        return 1;
    }

    // std::string input_file_path = "../tests/input.txt";
    // std::string output_file_encoded = "../tests/output_encoded.txt";
    // std::string output_file_decoded = "../tests/output_decoded.txt";
    

    std::ifstream in(inputFilePath, std::ios_base::in | std::ios_base::binary);

    if (!in.is_open()) {
        std::cout << "Error: input file could not open" << std::endl;
        return 1;
    }

    in.seekg(0, std::ios::end);
    std::streamsize size = in.tellg();
    in.seekg(0, std::ios::beg);


    
    std::ofstream out(outputFilePath, std::ios_base::out | std::ios_base::binary);

    if (!out.is_open()) {
        std::cout << "Error: output file could not open" << std::endl;
        return 2;
    }

    if (size == 0) {
        return 0;
    }


    if (compress == true) {
        size_t readedBytes = 0;
        while (readedBytes < size) {
            //uint bytesToRead = std::min(size - readedBytes, (size_t) 1024 * 1024 * 8);
            uint bytesToRead = std::min(size - readedBytes, (size_t) 100);
            std::vector<unsigned char> buffer(bytesToRead);

            if (!in.read(reinterpret_cast<char*>(buffer.data()), bytesToRead)) {
                std::cout << "Error: read" << std::endl;
                return 3;
            }
            readedBytes += bytesToRead;

            std::vector<unsigned char> encoded = BWT::encode(buffer);
            encoded = MTF::encode(encoded);
            encoded = RLE::encode(encoded);
            encoded = Huffman::encode(encoded);
            
            unsigned char bytes[4] = {static_cast<unsigned char>((encoded.size() >> 24) & 0xFF),
                                      static_cast<unsigned char>((encoded.size() >> 16) & 0xFF),
                                      static_cast<unsigned char>((encoded.size() >> 8) & 0xFF),
                                      static_cast<unsigned char>((encoded.size()) & 0xFF)};
            out << bytes[0];
            out << bytes[1];
            out << bytes[2];
            out << bytes[3];

            for (auto ch : encoded) {
                out << ch;
            }

            std::cout << "Encoded bytes: " << readedBytes << std::endl;
        }
    } else {
        size_t readedBytes = 0;
        while (readedBytes < size) {
            std::vector<unsigned char> bytes(4);
            if (!in.read(reinterpret_cast<char*>(bytes.data()), 4)) {
                std::cout << "Error: read" << std::endl;
                return 3;
            }
            uint bytesToRead = (static_cast<uint>(bytes[0]) << 24) |
                               (static_cast<uint>(bytes[1]) << 16) |
                               (static_cast<uint>(bytes[2]) << 8)  |
                               (static_cast<uint>(bytes[3]));
            
            std::vector<unsigned char> buffer(bytesToRead);

            if (!in.read(reinterpret_cast<char*>(buffer.data()), bytesToRead)) {
                std::cout << "Error: read" << std::endl;
                return 3;
            }
            readedBytes += bytesToRead + 4;
            
            std::vector<unsigned char> decoded = Huffman::decode(buffer);
            decoded = RLE::decode(decoded);
            decoded = MTF::decode(decoded);
            decoded = BWT::decode(decoded);

            for (auto ch : decoded) {
                out << ch;
            }

            std::cout << "Decoded bytes: " << readedBytes << std::endl;
        }
    }



    in.close();
    out.close();


    return 0;
}