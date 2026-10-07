//
//  Token.cpp
//  lispInterpreter
//
//  Created by Emanuele Ferrando 
//

#include "Token.h"
#include <iostream>

//Formats the output as (TAG_NAME, "WORD_TEXT") using the lookup table Token::tag2string
std::ostream& operator<<(std::ostream& os, const Token& t) {
    os << "(" << Token::tag2string[t.tag] << ",\"" << t.word << "\")";
    return os;
};
 
