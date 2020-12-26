#ifndef TOKEN
#define TOKEN

#include <string>
#include <iostream>
#include <list>
#include <vector>

#include "char.h"
#include "symbol_table.h"

struct Token{
    enum Type{
        BeginExp,
        EndExp,
        FloatingPoint,
        Operator,
        Keyword,
        Number,
        Unknown
    };

    bool dirty;
    Type type;
    unsigned int prior;
    std::string content;

    Token():
        prior(0), content(""), dirty(0)
    {

    }
    Token(const std::string& str, Token::Type t): 
        dirty(0), prior(0),content(str),type(t)    
    {}
    void print(){
        std::cout << content << "\t\t" << prior << '\t' << type << "  dirty:" << dirty<<'\n';
    }

};


class Tokenizer{
private:
    std::list<Token> tokens;

    const SymbolTable table;

    const OperatorTableEntry& find_operator(char);

    void wrap_expression(std::list<Token>::iterator);
    void parse_tokens(const std::string& str);
    void gc();
    void grammar_analysis();
    void prior_analysis();
    void assemble_tree();
public:
    Tokenizer(const std::string &str);
    // we consider as garbage every token with Char::Ohter content
    std::list<Token>& get_tokens(){return tokens;}

    static std::list<Token>::iterator find_expression_back(std::list<Token>::iterator front ,std::list<Token>::iterator back);
    static std::list<Token>::iterator find_expression_front(std::list<Token>::iterator front,std::list<Token>::iterator back);

    static std::list<Token>::iterator find_first_lowest(Token::Type, bool dirty, std::list<Token>::iterator begin,std::list<Token>::iterator end);
    static std::list<Token>::iterator find_last_lowest(Token::Type, bool dirty, std::list<Token>::iterator begin,std::list<Token>::iterator end);
    static std::list<Token>::iterator find_first_highest(Token::Type, bool dirty, std::list<Token>::iterator begin,std::list<Token>::iterator end);
    static std::list<Token>::iterator find_last_highest(Token::Type, bool dirty, std::list<Token>::iterator begin,std::list<Token>::iterator end);

    void print_tokens();
    void print_tokens_as_string();
    
    
};

#endif