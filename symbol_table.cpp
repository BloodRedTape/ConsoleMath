#include "symbol_table.h"


#include "local_math.h"
#include "char.h"

// here we link c++ functions and constants with our math "operators" functions and constants




// OPERATOR TABLE
// In this Parser Mathematical operators are represented by regular function with prototype "double name(double,double)"
// from here Parser takes C++ functions to use them for operators
// {[operator_symbol], [operator priority], [representer function pointer]}
// WARNING: don't use any kind of letter character for operator_symbol
// it will be interpreted as operator and give you an error
// WARNING: keep your functions in local_math.cpp file 
std::vector<OperatorTableEntry> SymbolTable::operator_table={
    {'+',1,sum},
    {'-',1,sub},
    {'*',2,mult},
    {'/',2,div},
    {'^',3,pow} //Recommended to have the biggest priority there
};

// FUNCTIONS TABLE
// from here Parser takes C++ functions to use them for mathematical functions
// {[parse_function_name],[representer_function_pointer]}
// WARNING: don't use any kind of non-letter character for parse_function_name
// it will be interpreted as operator and give you an error
// WARNING: keep your functions in local_math.cpp file 
std::vector<FunctionTableEntry> SymbolTable::function_table={
    {"sin",sin},
    {"cos",cos},
    {"tan",tan},
    {"sqrt",sqrt}
};

// CONSTANTS TABLE
// from here Parser takes values to replace constants
// {[constant_name] ,[constant_value]}
// WARNING: don't use any kind of non-letter character for constant_name
// it will be interpreted as operator and give you an errord
// WARNING: negative constants whould be enclosed in brackets
// example: {"negativeConst", "(-2)"}
// negative constants without brackets cause UNDEFINED BEHAVIOUR
std::vector<ConstsTableEntry> SymbolTable::consts_table={
    {"Pi","3.141"},
    {"e","2.71"}
};

