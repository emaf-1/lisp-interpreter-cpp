//
//  Lexer.h
//  lispInterpreter
//
//  Created by Emanuele Ferrando
//

#ifndef Lexer_h
#define Lexer_h

#include <vector>
#include <fstream>
#include <map>
#include <string>
#include "Token.h"
#include "Exceptions.h"

// Function object to tokenize an input stream
class Lexer {
public:
    Lexer();
    ~Lexer() = default;
    Lexer(Lexer const&) = delete;
    Lexer& operator = (Lexer const&) = delete;

    std::vector<Token> operator()(std::ifstream& inputFile) {
        std::vector<Token> inputTokens;
        tokenizeInputFile(inputFile, inputTokens);
        return inputTokens;
    }

private:
    std::string tokenizeConstant(std::ifstream& inputFile, std::stringstream& temp);
    void tokenizeInputFile(std::ifstream& inputFile, std::vector<Token>& inputTokens);

    std::map<std::string, int> keywords; // keyword -> tag
};

#endif /* Lexer_h */
