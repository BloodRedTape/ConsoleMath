#include "expression.h"
#include <iostream>

Expression::Expression(Operand * p_l, Operand * p_r, double (*func)(double, double)):
    p_lvalue(p_l), 
    p_rvalue(p_r),
    p_operator(func)
{}



double Expression::solve(){
    //std::cout << "r_val (" << p_rvalue->solve() << ") l_val (" << p_lvalue->solve() <<")";
    return p_operator(p_lvalue->solve(), p_rvalue->solve());
}

Expression::~Expression(){
    delete p_lvalue;
    delete p_rvalue;
}
