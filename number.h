#ifndef NUMBER_H
#define NUMBER_H

#include "expression.h"


// Yes, that is stupid but polymorphism forces me to do this :-(
class Number: public Operand{
private:
    double num;
public:
    Number(double n):
        num(n)
    {

    }
    ~Number(){
        
    }
    inline double solve()override{
        return num;
    }
};


#endif