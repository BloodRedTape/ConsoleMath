#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <memory>

class Operand{
public:
    virtual ~Operand(){
        
    }
    virtual double solve() = 0;
    
};


class Expression: public Operand{
public:
    Operand * p_lvalue;
    Operand * p_rvalue;
    double (*p_operator)(double, double);

public:
    Expression(Operand *lvalue, Operand *rvalue, double (*function)(double, double));
    void lvalue(Operand* l){p_lvalue=l;}
    void rvalue(Operand* r){p_rvalue=r;}
    void op(double (*opr)(double, double)){p_operator = opr;}
    double solve();
    ~Expression();
};



#endif