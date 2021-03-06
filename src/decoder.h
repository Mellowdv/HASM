#ifndef DECODER_H
#define DECODER_H

#include <string>
#include <unordered_map>
#include <fstream>
#include "parser.h"

// Forward declaration
class TokenStream;

// Helper function

bool isJump(std::string s);

const int num_of_registers = 16;

enum Registers {
    R0 = 0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15
};

class Decoder {
private:
    std::ofstream bin_file {};
    int current_ram_location {16};
    std::unordered_map<std::string, unsigned int> labels {};
    std::unordered_map<std::string, unsigned int> variables {};
    std::unordered_map<std::string, std::string> instructions {};
    std::unordered_map<std::string, std::string> jumps {};

public:
    Decoder(std::string file_name);
    void decode(TokenStream &ts);
    void add_variable();
    int var_address() const;
    std::string symbol();
    std::string dest(std::string s);
    std::string comp(std::string s);
    std::string jump(std::string s);
    void add_label(std::string s, int n);
    void print_labels();
    void print_variables();
    std::unordered_map<std::string, unsigned int> get_labels() const { return labels; }
};

#endif