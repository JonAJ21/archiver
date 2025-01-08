#include <iostream>

#include "rle.h"
#include "mtf.h"
#include "bwt.h"
#include "suffix_tree.h"
#include "sort.h"

int main() {

    std::string command;
    std::cin >> command;
    if (command == "compress") {
        std::string input;
    
        std::string str;
        while (std::cin >> str) {
            input += str;
        }

        if (input.size() == 0) {
            return 0;
        }

        std::string bwtOutput = BWT::encode(input);
        std::string mtfOutput = MTF::encode(bwtOutput);
        std::string rleOutput = RLE::encode(mtfOutput);

        std::cout << rleOutput << std::endl;

    } else if (command == "decompress") {
        std::string input;
        std::string str;
        while (std::cin >> str) {
            input += str + '\n';
        }        

        std::string rleOutput = RLE::decode(input);
        std::string mtfOutput = MTF::decode(rleOutput);
        std::string bwtOutput = BWT::decode(mtfOutput);
        
        std::cout << bwtOutput << std::endl;
    }

    return 0;
}