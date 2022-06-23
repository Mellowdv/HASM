#include "decoder.h"
#include "parser.h"
#include <iostream>
#include <bitset>

// Helper function

bool isJump(std::string s) {
    for (auto ch: s) {
        if (ch == ';')
            return true;
    }
    return false;
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

void Decoder::decode(TokenStream &ts) {
    Token t = ts.get_token();
    char inst_type {t.get_instr_type()};
    std::string instruction {t.get_symbol()};
    switch (inst_type) {
        case A_INSTRUCTION:
            if (isdigit(instruction.at(0))) {
                bin_file << std::bitset<16>{std::stoul(t.get_symbol())} << std::endl;
                break;
            }
            if (labels.find(instruction) != labels.end()) {
                bin_file << std::bitset<16>{labels.at(t.get_symbol())} << std::endl;
                break;
            }
            if (variables.find(instruction) != variables.end()) {
                bin_file << std::bitset<16>{variables.at(t.get_symbol())} << std::endl;
                break;
            }
            variables.emplace(instruction, current_ram_location);
            bin_file << "Added variable: " << t.get_symbol() << std::endl;
            current_ram_location++;
            break;
        case L_INSTRUCTION:
            bin_file << "Encountered L_Instruction" << std::endl;
            break;
        case C_INSTRUCTION:
            if (isJump(t.get_symbol()))
                bin_file << "It's a Jump Instruction" << std::endl;
            else
                bin_file << "111" << comp(instruction) << dest(instruction) << "000" << std::endl;
            break;
        default:
            bin_file << "Something unexpected happened" << std::endl;
            break;
    }
}

std::string Decoder::dest(std::string s) {
    std::string prep {};
    std::string bin_output {};
    for (auto ch: s) {
        if (ch == '=')
            break;
        prep += ch;
    }
    if (prep.find("A") != std::string::npos)
        bin_output += "1";
    else
        bin_output += "0";
    if (prep.find("D") != std::string::npos)
        bin_output += "1";
    else
        bin_output += "0";
    if (prep.find("M") != std::string::npos)
        bin_output += "1";
    else
        bin_output += "0";

    return bin_output;
}

std::string Decoder::comp(std::string s) {
    std::string prep {};
    std::string bin_output {};

    size_t prep_begin = s.find('=');
    for (int i = prep_begin; i < s.length(); i++) {
        prep += s.at(i);
    }
    
    int int_string_representation {stoi(s)};
    // checks the 'a' bit 
    if (int_string_representation == 1 || int_string_representation == -1 || int_string_representation == 0)
        bin_output += "1";
    else if (prep.find("M") != std::string::npos)
        bin_output += "1";
    else
        bin_output += "0";
    
    auto it = s.begin();

    return "0000000";
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