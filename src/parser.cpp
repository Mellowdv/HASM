#include "parser.h"
#include <sstream>
#include <iostream>

// helper functions

std::string prep_input(std::string s) {
    std::string result;
    for (auto it = s.begin(); it < s.end(); it++) {
        switch (*it) {
            case '(': case ')': {
                continue;
                break;
            }
            case '@': {
                continue;
                break;
            }
            default: {
                result += (*it);
                break;
            }
        }
    }
    return result;
}

// TokenStream

void TokenStream::add_token(std::string input, int rom_location) {
    // stuff happening in here
    // if @ token, create a token type A with empty string and value for pure number, name and value for symbolic
    auto it = input.begin();

    switch (*it) {
        case '@': {
            input = prep_input(input);
            line.push_back(Token{input, A_INSTRUCTION});
            break;
        }
    // if ( token, create a token type L with string name and value = current_mem_location, without the final )
        case '(': {
            input = prep_input(input);
            line.push_back(Token{input, L_INSTRUCTION, rom_location});
            break;
        }
    // if anything else, create a C type token with string representation included
        default: {
            line.push_back(Token{input, C_INSTRUCTION});
        }
    }
}

void TokenStream::clear_tokens() {
    line.clear();
}

char TokenStream::peek_type() const {
    if (line.empty()) {
        return 'F';
    }
    else
        return line.at(0).get_instr_type();
}

Token TokenStream::get_token() const {
    if (line.empty()) {
        throw std::runtime_error("Attempted to access empty vector");
    }
    else
        return line.at(0);
}



// Parser

Parser::Parser(std::string file_name) {
    asm_file.open(file_name);
    if (!(asm_file.is_open())) {
        throw std::runtime_error("Error opening file");
    }
}

void Parser::parse(TokenStream &ts) {
    advance(ts);
    Parser::first_pass(ts);
    asm_file.seekg(0); 
    Parser::second_pass(ts);
    asm_file.close();
}

bool Parser::has_more_lines() {
    return !(asm_file.eof());
}

void Parser::advance(TokenStream &ts) {
    // read the line from file
    // put it in a stringstream
    std::string input;
    std::stringstream asm_line;
    getline(asm_file, input);
    asm_line.str(input);

    // make tokens from the strings inside
    while (asm_line >> input) {
        auto it = input.begin();
        if ((*it) == '/' && (*(it+1) == '/')) {
            break;
        }
        ts.add_token(input, current_rom_location);
        current_rom_location++;
    }
    // move the counter forward to keep track of the location
}
void Parser::first_pass(TokenStream &ts) {
    // if instruction type != L, skip to the next line
    // if instruction is L type, add to the labels list
    // adding to the list happens on the first available memory location
    while (has_more_lines())
    {
        if (ts.peek_type() != 'L') {
            ts.clear_tokens();
            advance(ts);
        }
        // else add stuff to the hashmap
        else {
            add_label(ts);
            ts.clear_tokens();
            advance(ts);
        }
    }
}

void Parser::second_pass(TokenStream &ts) {
    // go through the file a second time
    // construct lines in TokenStream
    // call coder on the line to transform into binary
    // coder writes into the output file
}

void Parser::add_label(TokenStream &ts) {
    Token t = ts.get_token();
    labels.emplace(t.get_symbol(), t.get_value() + 1);   
}

void Parser::print_labels() {
    for (auto it: labels) {
        std::cout << it.first << " " << it.second << std::endl;
    }
}