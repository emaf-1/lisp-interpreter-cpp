//
//  Token.h
//  lispInterpreter
//
//  Created by Emanuele Ferrando 
//

#ifndef Token_h
#define Token_h

#include <string>

struct Token{
    //Tags
    static constexpr int LP         = 0;        // left parenthesis (
    static constexpr int RP         = 1;        // right parenthesis )
    static constexpr int BLOCK      = 2;        // BLOCK
    static constexpr int SET        = 3;        // SET
    static constexpr int PRINT      = 4;        // PRINT
    static constexpr int INPUT      = 5;        // INPUT
    static constexpr int IF         = 6;        // IF
    static constexpr int WHILE      = 7;        // WHILE
    static constexpr int ADD        = 8;        // +
    static constexpr int SUB        = 9;        // -
    static constexpr int MUL        = 10;       // *
    static constexpr int DIV        = 11;       // /
    static constexpr int GT         = 12;       // >
    static constexpr int LT         = 13;       // <
    static constexpr int EQ         = 14;       // =
    static constexpr int AND        = 15;       // &&
    static constexpr int OR         = 16;       // ||
    static constexpr int NOT        = 17;       // !=
    static constexpr int TRUE_KW    = 18;       // TRUE
    static constexpr int FALSE_KW   = 19;       // FALSE
    static constexpr int NUMBER     = 20;       // number
    static constexpr int VARIABLE_ID= 21;       // variable identifier
    

    //Mapping tag to readable strings
    static constexpr const char* id2word[]{
            "(", ")", "BLOCK", "SET", "PRINT", "INPUT", "IF", "WHILE", "ADD", "SUB", "MUL", "DIV", "GT", "LT", "EQ", "AND", "OR", "NOT", "TRUE", "FALSE", "NUMBER", "VARIABLE_ID"
        };
    
    //Tag to string name
    static constexpr const char* tag2string[]{
        "LP", "RP", "BLOCK", "SET", "PRINT", "INPUT", "IF", "WHILE", "OP", "OP", "OP", "OP", "RELOP", "RELOP", "RELOP", "LOGOP", "LOGOP", "LOGOP", "BOOL_CONST", "BOOL_CONST", "NUMBER", "VARIABLE_ID"
    };
    
    Token(int t, const char* w) : tag{ t }, word{ w } {}
    Token(int t, std::string w) : tag{ t }, word{ w } {}
    ~Token() = default;
    Token(Token const&) = default;
    Token& operator=(Token const&) = default;

    int tag;
    std::string word;
};

//Overloading the output stream operator for tokens
std::ostream& operator<<(std::ostream& os, const Token& t);


#endif /* Token_h */
