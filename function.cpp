#include "function.h"


Function::Function(Operand* op, double (*func)(double)):
    p_arg(op), impl(func)
{

}

double Function::solve(){
    return impl(p_arg->solve());
}