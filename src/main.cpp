#include <iostream>
#include <string>
#include "parser.h"
#include "decoder.h"

int main() {

    Parser parser("Pong.asm");
    Decoder decoder("Pong.hack");
    TokenStream ts;
    parser.parse(decoder, ts);
    return 0;
}