#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>

struct Char{
    enum Type{
        Digit,
        Letter,
        Symbol,
        Sys_Reserved,
        Other
    };

    static Type get_type(char);
    
    static bool is(char, Type);

    static double to_digit(std::string);
    
};

#endif