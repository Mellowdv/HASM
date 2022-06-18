#include <iostream>
#include <string>
#include "parser.h"
#include "decoder.h"

int main() {

    Parser parser("Pong.asm");
    Decoder decoder;
    TokenStream ts;
    parser.parse(decoder, ts);
    decoder.print_labels();
    decoder.print_variables();
    return 0;
}