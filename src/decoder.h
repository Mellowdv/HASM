#include "parser.h"
#include <map>

class Decoder {
private:
    std::ostream bin_file;
    std::map<std::string, int> variables;

public:
    Decoder();
    void decode(std::unordered_map<std::string, int>& labels);
    void add_variable();
    int var_address() const;
    std::string symbol();
    std::string dest();
    std::string comp();
    std::string jump();
};