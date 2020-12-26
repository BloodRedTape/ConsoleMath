#include "parse_tree.h"
#include "tokenizer.h"

ParseTree::ParseTree(std::list<Token>& tokens){
    root = parse(tokens.begin(),std::prev(tokens.end()));
}

ParseNode ParseTree::parse(std::list<Token>::iterator begin,std::list<Token>::iterator end){
    /*std::cout << "Parse: ";
    for(auto itr = begin; itr!= std::next(end); itr++){
        std::cout << itr->content;
    }
    std::cout << std::endl; */
    ParseNode node;
    std::list<Token>::iterator key = Tokenizer::find_last_lowest(Token::Type::Keyword,false,begin,end);
    std::list<Token>::iterator self = Tokenizer::find_first_lowest(Token::Type::Operator,true,begin,end);
    
    //if the lowest operator is inside function func(expr)
    if(key->prior<=self->prior && key->type==Token::Type::Keyword){
        
        if(std::prev(end)->type == Token::Type::Number){
            // case (number)
            node.self= &*std::prev(end);
        }else if(key->type==Token::Keyword){
            //case (func(other_expr))
            node.self=&*key;
            node.children.push_back(parse(std::next(key),Tokenizer::find_expression_back(std::next(key),end)));
        }else{
            // who knows what can there be
            std::cout << "Parse Tree: Undefined behaviour: Can't find operator, number or keyword in expression" << std::endl;
            exit(0);
        }
    // only number
    }else if(key == self){
        node.self= &*std::prev(end);
    
    // the lowest operator is outsize of the function => we haven't parsed other stuff expr+func(expr)
    }else{
        // self now is the lowest dirty operator
        node.self = &*self;
        switch (std::prev(self)->type)
        {
        case Token::Type::Number:
            node.children.push_back(ParseNode(&*std::prev(self)));
            break;
        case Token::Type::EndExp:
            node.children.push_back(parse(begin,std::prev(self)));
            break;
        
        default:
            std::cout << "Parse Tree: Undefined behaviour: lvalue "<<std::prev(self)->content<<" is wrong" << std::endl;
            exit(0);
            break;
        }
        switch (std::next(self)->type)
        {
        case Token::Type::Number:
            node.children.push_back(ParseNode(&*std::next(self)));
            break;
        case Token::Type::BeginExp:
            node.children.push_back(parse(std::next(self),Tokenizer::find_expression_back(std::next(self),end)));
            break;
        default:
            std::cout << "Parse Tree: Undefined behaviour: rvalue "<< std::next(self)->content <<" is wrong" << std::endl;
            exit(0);
            break;
        }
    }

    return node;
}


void ParseTree::print_tree(){
    root.print();

}