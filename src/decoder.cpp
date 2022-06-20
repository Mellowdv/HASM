#include "decoder.h"
#include "parser.h"
#include <iostream>
#include <bitset>

void Decoder::decode(TokenStream &ts) {
    Token t = ts.get_token();
    switch (t.get_instr_type()) {
        case A_INSTRUCTION:
            if (isdigit(t.get_symbol().at(0))) {
                bin_file << std::bitset<16>{std::stoul(t.get_symbol())} << std::endl;
                break;
            }
            if (labels.find(t.get_symbol()) != labels.end()) {
                bin_file << std::bitset<16>{labels.at(t.get_symbol())} << std::endl;
                break;
            }
            if (variables.find(t.get_symbol()) != variables.end()) {
                bin_file << std::bitset<16>{variables.at(t.get_symbol())} << std::endl;
                break;
            }
            variables.emplace(t.get_symbol(), current_ram_location);
            bin_file << "Added variable: " << t.get_symbol() << std::endl;
            current_ram_location++;
            break;
        case L_INSTRUCTION:
            bin_file << "Encountered L_Instruction" << std::endl;
            break;
        case C_INSTRUCTION:
            bin_file << "Encountered C_Instruction" << std::endl;
            break;
        default:
            bin_file << "Something unexpected happened" << std::endl;
            break;
    }
}

Decoder::Decoder(std::string file_name) {
    for (int i = 0; i < num_of_registers; i++) {
        std::string register_name {"R"+std::to_string(i)};
        variables.emplace(register_name, i);
    }
    variables.emplace("SP", 0);
    variables.emplace("LCL", 1);
    variables.emplace("ARG", 2);
    variables.emplace("THIS", 3);
    variables.emplace("THAT", 4);
    variables.emplace("SCREEN", 16384);
    variables.emplace("SP", 24576);

    bin_file.open(file_name);
}

void Decoder::add_label(std::string s, int n) {
    labels.emplace(s, n);  
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