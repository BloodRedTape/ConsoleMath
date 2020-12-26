#include <iostream>
#include <string>
#include "parser.h"
#include "char.h"
#include <cmath>
#include "unistd.h"
#include <memory>
//#include <chrono>

/*

HOW TO USE

have your expression in std::string
std::string expression("sin(34*Pi)/sqrt(4^3)-13");
//Create an instance of Parser class with expression string as argument
Parser parser(expression);
//call parse() function to generate mathematical expression tree and store it in Operand* pointer
Operand * math_expression = parser.parse();
//solve the expression and use result wherever yoy want
double res = math_expression->solve();

//to add your functions or const values into parser visit symbol_table.cpp file

*/

int main(int argc, char **argv){
    std::string expression;
    if(argc == 1){
        std::cout << "Enter mathematical expression: ";
        std::getline(std::cin, expression); // read line from stdin
    }else if (argc == 2){
        expression=(std::string&&)std::string(argv[1]);
    }else{
        for(int i = 1; i<argc; i++){
            expression += std::string(argv[i]);
        }
    }
    Parser pars(expression);
    std::unique_ptr<Operand>expr(pars.parse());
    std::cout << "Result: " << expr->solve() << std::endl;
}

