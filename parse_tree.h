#ifndef PARSE_TREE
#define PARSE_TREE

#include "tokenizer.h"
#include <iostream>

struct ParseNode{
    Token* self;
    std::list<ParseNode> children;
    ParseNode():
        self(nullptr)
    {}
    ParseNode(Token* t):
        self(t)
    {}
    void print(){
        if(self!=nullptr){
            std::cout << "self: "<< self->content << std::endl;
            std::cout << "children \n";
            for(ParseNode& node: children){
                node.print();
            }
        }
    }  
};

class ParseTree{
private:
    ParseNode root;
    ParseNode parse(std::list<Token>::iterator,std::list<Token>::iterator);
    
public:
    ParseTree(std::list<Token>&);
    void print_tree();
    ParseNode& get_tree(){return root;} 
};

#endif