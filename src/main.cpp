#include <iostream>
#include <string>
#include "parser.h"
#include "decoder.h"

int main(int argc, char **argv) {

    Parser parser(argv[1]);
    std::string file {argv[1]};
    std::string output_file {};
    auto it = file.begin();
    for (it; it < file.end(); it++) {
        if (*it == '.') {
            break;
        }
        output_file += *it;
    }
    Decoder decoder(output_file + ".hack");
    TokenStream ts;
    parser.parse(decoder, ts);
    return 0;
}