//
//  Lexer.cpp
//  lispInterpreter
//
//  Created by Emanuele Ferrando
//

#include <sstream>
#include <cctype>
#include <cstdint>
#include "Lexer.h"

Lexer::Lexer() {
    //Constructor: maps all keywords to their corresponding Token IDs
    keywords["BLOCK"] = Token::BLOCK;
    keywords["SET"]   = Token::SET;
    keywords["PRINT"] = Token::PRINT;
    keywords["INPUT"] = Token::INPUT;
    keywords["IF"]    = Token::IF;
    keywords["WHILE"] = Token::WHILE;
    keywords["GT"]    = Token::GT;
    keywords["LT"]    = Token::LT;
    keywords["EQ"]    = Token::EQ;
    keywords["AND"]   = Token::AND;
    keywords["OR"]    = Token::OR;
    keywords["NOT"]   = Token::NOT;
    keywords["TRUE"]  = Token::TRUE_KW;
    keywords["FALSE"] = Token::FALSE_KW;
    keywords["ADD"]   = Token::ADD;
    keywords["SUB"]   = Token::SUB;
    keywords["MUL"]   = Token::MUL;
    keywords["DIV"]   = Token::DIV;
}

//Method to consume and build a sequence of numeric digits
std::string Lexer::tokenizeConstant(std::ifstream& inputFile, std::stringstream& temp) {
    char ch = inputFile.get();
    while (std::isdigit(ch)) {
        temp << ch;
        ch = inputFile.get();
    }
    // Unget last character
    inputFile.unget();
    return temp.str();
}

//Scan input file and populates the token vector
void Lexer::tokenizeInputFile(std::ifstream& inputFile, std::vector<Token>& inputTokens) {
    char ch{ };
    unsigned int rowCount{ 1 };
    ch = inputFile.get();
    
    while (inputFile) {
        if (std::isspace(ch)) {
            // Skip newline
            if (ch == '\n') rowCount += 1;
        }
        else if (ch == '(') {       //if '(' -> left parenthesis
            inputTokens.push_back(Token{ Token::LP, Token::id2word[Token::LP] });
        }
        else if (ch == ')') {       //if ')' -> right parenthesis
            inputTokens.push_back(Token{ Token::RP, Token::id2word[Token::RP] });
        }
        else if (ch == '-' && std::isdigit(inputFile.peek())) {
            // Negative number
            ch = inputFile.get();
            std::string digits;
            while (std::isdigit(ch)) {
                digits += ch;
                ch = inputFile.get();
            }
            inputFile.unget();

            // posnumber -> 0 | sigdigit rest -> a number starting with '0' is only valid if it's only "0" and a number must be followed by a delimiter: if it's immediately followed by another character (letter), it's not valid
            int next = inputFile.peek();
            bool isDelimiter = (next == EOF) || !std::isalnum(static_cast<unsigned char>(next));
            bool invalidLeadingZero = (digits[0] == '0' && digits.size() > 1);

            if (invalidLeadingZero || !isDelimiter) {
                std::string word = "-" + digits;
                char c = inputFile.get();
                while (std::isalnum(static_cast<unsigned char>(c))) {
                    word += c;
                    c = inputFile.get();
                }
                inputFile.unget();
                std::stringstream err;
                err << "Invalid word '" << word << "' at line " << rowCount;
                throw LexicalError{ err.str() };
            }
            inputTokens.push_back(Token{ Token::NUMBER, "-" + digits });
        }
        
        else if (std::isdigit(ch)) {
            // Numeric constant
            std::stringstream temp;
            temp << ch;
            tokenizeConstant(inputFile, temp);
            std::string digits = temp.str();

            // posnumber -> 0 | sigdigit rest -> a number starting with '0' is only valid if it's only "0" and a number must be followed by a delimiter: if it's immediately followed by another character (letter), it's not valid
            int next = inputFile.peek();
            bool isDelimiter = (next == EOF) || !std::isalnum(static_cast<unsigned char>(next));
            bool invalidLeadingZero = (digits[0] == '0' && digits.size() > 1);

            if (invalidLeadingZero || !isDelimiter) {
                std::string word = digits;
                char c = inputFile.get();
                while (std::isalnum(static_cast<unsigned char>(c))) {
                    word += c;
                    c = inputFile.get();
                }
                inputFile.unget();
                std::stringstream err;
                err << "Invalid word '" << word << "' at line " << rowCount;
                throw LexicalError{ err.str() };
            }
            inputTokens.push_back(Token{ Token::NUMBER, digits });
        }
        else if (std::isalpha(ch)) {
            // Keyword or ID (variable)
            std::stringstream temp;
            temp << ch;
            bool hasDigit = false;
            do {
                ch = inputFile.get();
                if (std::isalpha(ch)) {
                    temp << ch;
                }
                else if (std::isdigit(ch)) {
                    temp << ch;
                    hasDigit = true;
                }
            } while (std::isalpha(ch) || std::isdigit(ch));
            
            std::string word{ temp.str() };
            
            //Rejects words with number inside
            if (hasDigit) {
                std::stringstream err;
                err << "Invalid word '" << word << "' at line " << rowCount;
                throw LexicalError{ err.str() };
            }
            //Check if identifier matches a reserved keyword or a variable
            auto it = keywords.find(word);  //compiler knows that it is an iterator of std::map<std::string, int>
            if (it != keywords.end()) {
                //Keyword
                inputTokens.push_back(Token{ it->second, word });
            }
            else {
                //ID
                inputTokens.push_back(Token{ Token::VARIABLE_ID, word });
            }
            
            // Unget last character
            inputFile.unget();
        }
        else {
            // Garbage
            std::stringstream temp;
            temp << "Stray character " << ch
            << " in input at line " << rowCount;
            throw LexicalError{ temp.str() };
        }
        ch = inputFile.get();
    }
}
