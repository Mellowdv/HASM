#include "decoder.h"
#include "parser.h"
#include <iostream>
#include <bitset>

// Helper functions

bool is_jump(std::string s) {
    for (auto ch: s) {
        if (ch == ';')
            return true;
    }
    return false;
}

void load_instructions(std::unordered_map<std::string, std::string> &instructions) {
    instructions.emplace("0", "101010");
    instructions.emplace("1", "111111");
    instructions.emplace("-1", "111010");
    instructions.emplace("D", "001100");
    instructions.emplace("A", "110000");
    instructions.emplace("M", "110000");
    instructions.emplace("!D", "001101");
    instructions.emplace("!A", "110001");
    instructions.emplace("!M", "110001");
    instructions.emplace("-D", "001111");
    instructions.emplace("-A", "110011");
    instructions.emplace("-M", "110011");
    instructions.emplace("D+1", "011111");
    instructions.emplace("A+1", "110111");
    instructions.emplace("M+1", "110111");
    instructions.emplace("D-1", "001110");
    instructions.emplace("A-1", "110010");
    instructions.emplace("M-1", "110010");
    instructions.emplace("D+A", "000010");
    instructions.emplace("D+M", "000010");
    instructions.emplace("D-A", "010011");
    instructions.emplace("D-M", "010011");
    instructions.emplace("A-D", "000111");
    instructions.emplace("M-D", "000111");
    instructions.emplace("D&A", "000000");
    instructions.emplace("D&M", "000000");
    instructions.emplace("D|A", "010101");
    instructions.emplace("D|M", "010101");
}

void load_jumps(std::unordered_map<std::string, std::string> &jumps) {
    jumps.emplace("JGT", "001");
    jumps.emplace("JEQ", "010");
    jumps.emplace("JGE", "011");
    jumps.emplace("JLT", "100");
    jumps.emplace("JNE", "101");
    jumps.emplace("JLE", "110");
    jumps.emplace("JMP", "111");
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
    load_instructions(instructions);
    load_jumps(jumps);

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
            bin_file << std::bitset<16>{variables.at(instruction)} << std::endl;
            current_ram_location++;
            break;
        case L_INSTRUCTION:
            break;
        case C_INSTRUCTION:
            if (is_jump(t.get_symbol()))
                bin_file << "111" << jump(instruction) << std::endl;
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

    size_t prep_begin = s.find('=') + 1;
    if (prep_begin != std::string::npos) {
        for (int i = prep_begin; i < s.length(); i++) {
            prep += s.at(i);
        }
    }
    else
        prep = s;

    if (prep.find('M') != std::string::npos)
        bin_output += "1";
    else
        bin_output += "0";
    bin_output += instructions.at(prep);
    return bin_output;
}

std::string Decoder::jump(std::string s) {
    std::string prep {};
    std::string bin_output {};
    std::string comp {};

    auto prep_start = s.find(';');
    comp = s.substr(0, prep_start);
    prep = s.substr(prep_start+1, s.length()-1);

    comp = Decoder::comp(comp);
    prep = jumps.at(prep);

    bin_output = comp + "000" + prep;
    return bin_output;
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