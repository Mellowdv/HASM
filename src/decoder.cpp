#include "decoder.h"
#include "parser.h"
#include <iostream>
#include <bitset>

void Decoder::decode(TokenStream &ts) {
    Token t = ts.get_token();
    switch (t.get_instr_type()) {
        case A_INSTRUCTION:
            if (labels.find(t.get_symbol()) != labels.end()) {
                std::cout << t.get_symbol() << " " << std::bitset<16>{labels.at(t.get_symbol())} << std::endl;
                break;
            }
            if (variables.find(t.get_symbol()) != variables.end()) {
                std::cout << t.get_symbol() << " " << std::bitset<16>{variables.at(t.get_symbol())} << std::endl;
                break;
            }
            variables.emplace(t.get_symbol(), current_ram_location);
            std::cout << "Added variable: " << t.get_symbol() << std::endl;
            current_ram_location++;
            break;
        case L_INSTRUCTION:
            std::cout << "Encountered L_Instruction" << std::endl;
            break;
        case C_INSTRUCTION:
            std::cout << "Encountered C_Instruction" << std::endl;
            break;
    }
}

Decoder::Decoder() {
    for (int i = 0; i < num_of_registers; i++) {
        std::string register_name {"R"+std::to_string(i)};
        variables.emplace(register_name, i);
    }
}

void Decoder::add_label(std::string s, int n) {
    labels.emplace(s, n+1);  
}

void Decoder::print_labels() {
    for (auto it: labels) {
        std::cout << it.first << " " << it.second << std::endl;
    }
}

void Decoder::print_variables() {
    for (auto it: variables) {
        std::cout << it.first << " " << std::bitset<16>{it.second} << std::endl;
    }
}