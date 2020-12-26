#include "parser.h"
#include <cmath>
#include <iostream>
#include <queue>
#include "symbol_table.h"
#include "parse_tree.h"
#include "char.h"



Parser::Parser(const std::string& exp):
    tokenizer(exp),
    parse_tree(tokenizer.get_tokens())
{
    
}




Operand * Parser::generate_operand(ParseNode* node){
    switch (node->self->type)
    {
    case Token::Number:
        return new Number(Char::to_digit(node->self->content));
        break;
    case Token::Operator:
        if(node->children.size()!=2)
        {
            /*if(node->self->content[0]=='-'){
                return new Expression(new Number(0),generate_operand(&*node->children.begin()),SymbolTable::find_operator('-'));
            }*/
            std::cout << "Parser, expression should have two operands!" << std::endl;
            exit(0);
        } 

        return new Expression(generate_operand(&*node->children.begin()),generate_operand(&*std::next(node->children.begin())),SymbolTable::find_operator(node->self->content[0]));
    case Token::Keyword:
        return new Function(generate_operand(&*node->children.begin()), SymbolTable::find_function(node->self->content));
        break;
    default:
        std::cout << "Parser: Undefined behaviour" << std::endl;
        exit(0);
        break;
    }

}

Operand *Parser::parse(){

    //tokenizer.print_tokens();
    //parse_tree.print_tree();
    ParseNode tree = parse_tree.get_tree();
    return generate_operand(&tree);  
}







