#ifndef PARSER
#define PARSER

#include <string>
#include <vector>
#include "expression.h"
#include "function.h"
#include "number.h"
#include "tokenizer.h"

#include "symbol_table.h"
#include "parse_tree.h"

class Parser{
private:

    Tokenizer tokenizer;
    ParseTree parse_tree;
    SymbolTable table;
    

    
    Operand* generate_operand(ParseNode*); 
public:
    Parser(const std::string& exp);
    Operand *parse();

    
    
    
    


};




#endif