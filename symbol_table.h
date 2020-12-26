#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include <vector>
#include <string>
#include <iostream>

typedef double (*OperatorProc)(double,double);
typedef double (*FunctionProc)(double);

struct OperatorTableEntry{
    char symbol;
    int priority;
    OperatorProc func;
};

struct FunctionTableEntry{
    std::string symbols;
    FunctionProc func;
};

struct ConstsTableEntry{
    std::string symbols;
    std::string value;
};

struct SymbolTable{
    static std::vector<OperatorTableEntry> operator_table;
    static std::vector<FunctionTableEntry> function_table;
    static std::vector<ConstsTableEntry> consts_table;
    SymbolTable(){}
    static OperatorProc find_operator(char c){
        for(auto itr = operator_table.begin(); itr!=operator_table.end();itr++){
            if(itr->symbol==c){
                return itr->func;
            }
        }
        std::cout << "Symbol table: operator " << c << " not found" << std::endl;
        exit(0);
        return nullptr;

    }

    static FunctionProc find_function(const std::string& str){
        for(auto itr = function_table.begin();itr!=function_table.end();itr++){
            if(itr->symbols==str)
                return itr->func;
        }
        std::cout << "Symbol table: function " << str << " not found" << std::endl;
        exit(0);
        return nullptr;
    }

    static std::string* find_const(const std::string& str){
        for(auto itr = consts_table.begin(); itr!=consts_table.end();itr++){
            if(itr->symbols==str){
                return &itr->value;
            }
        }
        std::cout << "Symbol table: constant " << str << " not found" << std::endl;
        exit(0);
        return nullptr;
    }

};

#endif