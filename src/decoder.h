#include "parser.h"

class Decoder {
private:
    std::ostream bin_file;

public:
    Decoder();
    void decode();
    std::string symbol();
    std::string dest();
    std::string comp();
    std::string jump();
};