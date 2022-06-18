#include "decoder.h"
#include "parser.h"
#include <iostream>
#include <bitset>

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