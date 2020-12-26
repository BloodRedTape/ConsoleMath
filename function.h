#ifndef FUNCTION_H
#define FUNCTION_H



#include "expression.h"
#include "number.h"

class Function: public Operand{
private:
    Operand* p_arg;
    double (*impl)(double);
public:
    Function(Operand*, double (*func)(double));
    ~Function(){
        delete p_arg;
    }
    double solve()override;

};


#endif