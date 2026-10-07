//
//  Exceptions.h
//  lispInterpreter
//
//  Created by Emanuele Ferrando
//

#ifndef Exceptions_h
#define Exceptions_h

#include <stdexcept>
#include <string>


struct LexicalError : std::runtime_error {          //tokenizer / lexer errors
    LexicalError(const char* msg) : std::runtime_error(msg) {}
    LexicalError(std::string msg) : std::runtime_error(msg.c_str()) {}
};

struct SyntaxError : std::runtime_error {           //parser errors
    SyntaxError(const char* msg) : std::runtime_error(msg) {}
    SyntaxError(std::string msg) : std::runtime_error(msg.c_str()) {}
};


struct EvaluationError : std::runtime_error{        //visitor / evaluator errors
    EvaluationError(const char* msg) : std::runtime_error(msg) {}
    EvaluationError(std::string msg) : std::runtime_error(msg.c_str()) {}
};



#endif /* Exceptions_h */
