#include "char.h"
#include <cmath>
#include <iostream>

Char::Type Char::get_type(char c){
    if(c <=31){
        return Type::Sys_Reserved;
    }else if ((c >=33 && c<=47)||(c >= 58 && c<=64)||(c>=91 && c<=96)||(c >= 123 && c <= 126)){
        return Type::Symbol;
    }else if ( c >= 48 && c <= 57){
        return Type::Digit;
    }else if ((c>=65 && c <=90)||(c>=97 && c<=122)){
        return Type::Letter;
    }else{
        return Type::Other;
    }
}


bool Char::is(char c, Char::Type t){
    switch (t)
    {
    case Type::Sys_Reserved:
        if(c <=31){
            return true;
        }else{
            return false;
        }
        break;
    case Type::Symbol:
        if((c >=33 && c<=47)||(c >= 58 && c<=64)||(c>=91 && c<=96)||(c >= 123 && c <= 126)){
            return true;
        }else{
            return false;
        }
        break;
    case Type::Digit:
        if( c >= 48 && c <= 57){
            return true;
        }else{
            return false;
        }
        break;
    case Type::Letter:
        if((c>=65 && c <=90)||(c>=97 && c<=122)){
            return true;
        }else{
            return false;
        }
        break;
    case Type::Other:
        if(c >= 127 || c == 32){
            return true;
        }else{
            return false;
        }
        break;
    default:
        return false;
        break;
    }
}



double Char::to_digit(std::string str){
    double result = 0;
    int point = 0;
    for(int i = 0; i<str.size();i++){
        if(str[i]=='.'){
            point=i;
        }
    }
    if(point==0)
        point=str.size();
    for(int i = 0; i<point; i++){
        result+=(pow(10,point-i-1)*(str[i]-48));
    }
    for(int i = point+1; i<str.size();i++){
        result+=double(str[i]-48)/pow(10,i-point);
    }
    //std::cout << "string " << str << "interpreted as " << result << std::endl;
    return result;
}