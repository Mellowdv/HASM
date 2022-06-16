#include <iostream>
#include "parser.h"

int main() {

    Parser parser("Pong.asm");
    TokenStream ts;
    parser.parse(ts);
    parser.print_labels();
    return 0;
}