#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

const char A_INSTRUCTION = 'A';
const char L_INSTRUCTION = 'L';
const char C_INSTRUCTION = 'C'; 

class Token {
private:
    std::string symbol;
    char instruction_type;
    int value;

public:
    Token(std::string s, char inst_type): symbol{s}, instruction_type{inst_type} {}
    Token(std::string s, char inst_type, int mem_loc): symbol{s}, instruction_type{inst_type}, value{mem_loc} {}
    // getters and setters
    std::string get_symbol() const { return symbol; }
    int get_value() const { return value; }
    char get_instr_type() const { return instruction_type; }
    void set_symbol(std::string s) { symbol = s; }
    void set_value(int n) { value = n; }
    void set_instr_type(char c) { instruction_type = c; }

};

class TokenStream {
private:
    std::vector<Token> line {};

public:
    TokenStream() {}
    void add_token(std::string input, int rom_location);
    void clear_tokens();
    Token get_token() const;
    char peek_type() const;
};

class Parser {
private:
    std::ifstream asm_file {};
    std::unordered_map<std::string, int> labels {};
    int current_rom_location {};

public:
    Parser(std::string file_name);
    void parse(TokenStream &ts);
    bool has_more_lines();
    void advance(TokenStream &ts);
    void first_pass(TokenStream &ts);
    void second_pass(TokenStream &ts);
    void add_label(TokenStream &ts);
    void print_labels();
};
