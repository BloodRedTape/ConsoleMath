#include "tokenizer.h"
#include <iostream>
#include <queue>

#include "parser.h"

Tokenizer::Tokenizer(const std::string& str):
    table()
{
    parse_tokens(str);
    gc();
    grammar_analysis();
    prior_analysis();
    assemble_tree();
}

std::list<Token>::iterator Tokenizer::find_first_lowest(Token::Type type, bool dirty, std::list<Token>::iterator begin,std::list<Token>::iterator end){
    int lowest = 999999;
    std::list<Token>::iterator token = end;
    for(std::list<Token>::iterator itr = begin; itr!= end; itr++){
        if(itr->prior < lowest && itr->dirty == dirty && itr->type== type){
            token = itr;
            lowest = token->prior;
        }
    }
    return token;
}

std::list<Token>::iterator Tokenizer::find_last_lowest(Token::Type type, bool dirty, std::list<Token>::iterator begin,std::list<Token>::iterator end){
    int lowest = 999999;
    std::list<Token>::iterator token = end;
    for(std::list<Token>::iterator itr = begin; itr!= end; itr++){
        if(itr->prior <= lowest && itr->dirty == dirty && itr->type== type){
            token = itr;
            lowest = token->prior;
        }
    }
    return token;
}


std::list<Token>::iterator Tokenizer::find_first_highest(Token::Type type, bool dirty, std::list<Token>::iterator begin,std::list<Token>::iterator end){
    int highest = 0;
    std::list<Token>::iterator token = end;
    for(std::list<Token>::iterator itr = begin; itr!= end; itr++){
        if(itr->prior > highest && itr->dirty == dirty && itr->type== type){
            token = itr;
            highest = token->prior;
        }
    }
    return token;
}

std::list<Token>::iterator Tokenizer::find_last_highest(Token::Type type, bool dirty, std::list<Token>::iterator begin,std::list<Token>::iterator end){
    int highest = 0;
    std::list<Token>::iterator token = end;
    for(std::list<Token>::iterator itr = begin; itr!= end; itr++){
        if(itr->prior >= highest && itr->dirty == dirty && itr->type== type){
            token = itr;
            highest = token->prior;
        }
    }
    return token;
}


std::list<Token>::iterator Tokenizer::find_expression_back(std::list<Token>::iterator begin, std::list<Token>::iterator end){
    int indirection = 1;
    do{
        begin++;
        if(begin->type==Token::Type::BeginExp)
            ++indirection;
        else if(begin->type==Token::Type::EndExp)
            --indirection;
        else if(begin == end)
            return begin;
    }while(indirection!=0);
    return begin;
}

std::list<Token>::iterator Tokenizer::find_expression_front(std::list<Token>::iterator begin, std::list<Token>::iterator end){
    int indirection = 1;
    do{
        end--;
        if(end->type==Token::Type::BeginExp)
            --indirection;
        else if(end->type==Token::Type::EndExp)
            ++indirection;
        else if(end == begin)
            return end;
    }while(indirection!=0);
    return end;
}


const OperatorTableEntry& Tokenizer::find_operator(char c){
    for(std::vector<OperatorTableEntry>::const_iterator itr = table.operator_table.begin(); itr != table.operator_table.end(); itr++){
        if(itr->symbol == c){
            return *itr;
        }
    }
    std::cout << "Tokenizer: Can't create token: operator " << c << " not found" << std::endl;
    exit(0);
}

void Tokenizer::parse_tokens(const std::string& str){
    tokens.clear();
    for(int i = 0; i<str.size();){
        Token token;
        Char::Type type = Char::get_type(str[i]);
        int j = 0;
        if(type != Char::Type::Symbol){
            for(j; Char::is(str[i+j],type);j++){
                token.content+=str[i+j];
            }
        }else{
            token.content+=str[i+j];
            j++;
        }
        i+=j;
        tokens.push_back(token);
    }
}

void Tokenizer::gc(){
    std::queue<std::list<Token>::iterator> garbage;
    for(std::list<Token>::iterator itr = tokens.begin(); itr != tokens.end();itr++){
        Char::Type t = Char::get_type(itr->content[0]);
        if(t==Char::Type::Other || t == Char::Type::Sys_Reserved){
            garbage.push(itr);
        }
    }
    while(!garbage.empty()){
        tokens.erase(garbage.front());
        garbage.pop();
    }
}

// Set token types and merge parts of floating point number
void Tokenizer::grammar_analysis(){
    std::queue<std::list<Token>::iterator> dafp;
    for (std::list<Token>::iterator t = tokens.begin(); t!=tokens.end(); t++){
        if(Char::get_type(t->content[0])!=Char::Type::Letter){
            if(Char::get_type(t->content[0])==Char::Type::Digit){
                t->type=Token::Type::Number;
            }else if(t->content.size()==1){
                switch (t->content[0])
                {
                case '(':
                    t->type=Token::Type::BeginExp;
                    break;
                case ')':
                    t->type=Token::Type::EndExp;
                    break;
                case '.':
                    t->type=Token::Type::FloatingPoint;
                    dafp.push(t);
                    break;
                default:
                    t->type = Token::Type::Operator;
                    break;
                }
            }else{
                std::cout << "Grammar: operator consist from two symbols " << t->content << std::endl;
                exit(0);
            }
        }else{
            if(Char::get_type(t->content[0])==Char::Digit){
                t->type=Token::Type::Number;
            }else{
                t->type=Token::Type::Keyword;
            }
        }
    }
    while(!dafp.empty()){
        std::list<Token>::iterator point, next, prev;
        point = dafp.front();
        next = std::next(dafp.front());
        prev = std::prev(dafp.front());
        if(prev->type!=Token::Type::Number || next->type!=Token::Type::Number){
            std::cout << "Grammar Analisys failed, expexted a floating point at " << prev->content << point->content << next->content << std::endl;
            exit(0);
        }
        prev->content += point->content;
        prev->content += next->content;
        tokens.erase(point);
        tokens.erase(next);
        dafp.pop();
        
    }
}

// Priority evaluation based on expressions order position and operator prioriy
void Tokenizer::prior_analysis(){

    int prior = 0;
    int op_prior = 0;
    for(std::list<Token>::iterator itr = tokens.begin(); itr!=tokens.end(); itr++){
        switch (itr->type)
        {
        case Token::Type::BeginExp:
            prior += table.operator_table.size();
            itr->prior = prior + table.operator_table.size();
            break;
        case Token::Type::EndExp:
            itr->prior = prior + table.operator_table.size();
            prior -= table.operator_table.size();
            break;
        case Token::Type::Keyword:
            itr->prior = prior + table.operator_table.size();
            break;
        case Token::Type::Operator:
            op_prior = find_operator(itr->content[0]).priority;
            itr->prior = prior+op_prior;
            break;
            //this shoild never happen but i want to handle this case
        case Token::Type::Unknown:
            std::cout << "Parser: Grammar Analysis: unknown symbol " <<  itr->content << std::endl;
            exit(0);
            break;
        default:
            itr->prior=prior;
            break;
        }
    }
}

// wrap operator to form rigth order of calculation
void Tokenizer::wrap_expression(std::list<Token>::iterator t){
    std::list<Token>::iterator next=std::next(t);
    std::list<Token>::iterator prev=std::prev(t);

    std::list<Token>::iterator temp;
    switch (next->type)
    {
    case Token::Type::Number:
        tokens.insert(std::next(next),Token(")",Token::Type::EndExp));
        break;
    case Token::Type::BeginExp:
        tokens.insert(std::next(find_expression_back(next,tokens.end())),Token(")",Token::Type::EndExp));
        break;
    default:
        break;
    }
    switch (prev->type)
    {
    case Token::Type::Number:
        tokens.insert(prev,Token("(",Token::Type::BeginExp));
        break;
    case Token::Type::EndExp:
        temp=find_expression_front(tokens.begin(),prev);
        tokens.insert(temp,Token("(",Token::Type::BeginExp));
    default:
        break;
    }
}

// THE MOST OVERWEIGTH AND OPTIMIZATION_LESS FUNCTION
// create an ordered math expression that can be transformed into parse tree
void Tokenizer::assemble_tree(){
    std::list<Token>::iterator t;
    if(tokens.size()==1){
        std::cout << "Tokenizer: please enter a valid expression" << std::endl;
        exit(0);
    }

    //wrap functions and replace constants with their values
    for(auto itr = tokens.begin(); itr!=tokens.end(); itr++){
        if(itr->type==Token::Type::Keyword){
            if(std::next(itr)->type==Token::Type::BeginExp && std::next(itr)!= tokens.end()){
                tokens.insert(itr,Token("(",Token::Type::BeginExp));
                std::list<Token>::iterator end = find_expression_back(std::next(itr),tokens.end());
                tokens.insert(end,Token(")",Token::Type::EndExp));
                
            }else{
                itr->content=*SymbolTable::find_const(itr->content);
                itr->type=Token::Type::Number;
            }
            prior_analysis();
        }
    } 
    //deal with negative numbers
    for(std::list<Token>::iterator itr = tokens.begin(); itr!=tokens.end();itr++){
        if(itr->type==Token::Type::Operator && itr->content[0]=='-'){
            auto prev = std::prev(itr);
            if(prev->type == Token::Type::BeginExp || itr==tokens.begin()){
                itr->dirty = true;
                tokens.insert(itr, Token("0",Token::Type::Number));
                wrap_expression(itr);
                prior_analysis();
            }
        }
    }


/* MARKING BRACKETS LOSE ACCURANCY IN FLOATING POINT CALCULATION BUT INCREASE SPEED
    // mark brackets operators dirty not to have double wrapper like this ((3+3))
    for(auto itr = tokens.begin(); itr!=tokens.end();itr++){
        if(itr->type == Token::Type::BeginExp){
            find_last_lowest(Token::Type::Operator,false,itr,find_expression_back(itr,tokens.end()))->dirty=true;
        }
    }
    */

    while((t=find_first_highest(Token::Type::Operator,false,tokens.begin(),tokens.end()))!=tokens.end()){
        //Wrap expression with highest priority to form calculation order 2+2+2+2 => (((2+2)+2)+2)
        t->dirty = true;
        wrap_expression(t);
        
        
        //grammar_analysis(); IF SOMETHING GOES
        //recalculate priority after expression wrapance
        prior_analysis();
    }
}


void Tokenizer::print_tokens(){
    for(Token t: tokens){
        t.print();
    }
}

void Tokenizer::print_tokens_as_string(){
    for(Token t: tokens){
        std::cout<<t.content;
    }
    std::cout << std::endl;
}


