//
//  Parser.h
//  lispInterpreter
//
//  Created by Emanuele Ferrando
//

#ifndef Parser_h
#define Parser_h

#include <vector>
#include "Token.h"
#include "Syntax.h"
#include "Exceptions.h"

class Parser {
public:
    //Entry point -> parses the token vector into a full AST representation
    Program* operator()(std::vector<Token> const& tokenStream) {
        auto itr = tokenStream.begin();
        end_ = tokenStream.end();
        
        if (itr == end_) {
            throw SyntaxError{ "Empty program" };
        }
        
        Block* b = parseStmtBlock(itr);
        if (itr != end_) {
            throw SyntaxError{ "Unexpected tokens after end of program." };
        }
        return new Program{ b };
    }

private:
    std::vector<Token>::const_iterator end_;

    // Invariant: all parsing functions take itr at the beginning
    // of the symbol parsed and leave itr right after the end of the symbol

    // Statement block & statement parsers
    Block*      parseStmtBlock(std::vector<Token>::const_iterator& itr);
    Statement*  parseStatement(std::vector<Token>::const_iterator& itr);

    // Specific statement parsers
    SetStmt*    parseSetStmt(std::vector<Token>::const_iterator& itr);
    PrintStmt*  parsePrintStmt(std::vector<Token>::const_iterator& itr);
    InputStmt*  parseInputStmt(std::vector<Token>::const_iterator& itr);
    IfStmt*     parseIfStmt(std::vector<Token>::const_iterator& itr);
    WhileStmt*  parseWhileStmt(std::vector<Token>::const_iterator& itr);

    // Expression parsers
    NumExpr*    parseNumExpr(std::vector<Token>::const_iterator& itr);
    BoolExpr*   parseBoolExpr(std::vector<Token>::const_iterator& itr);

    //Leaf node parsers
    Variable*   parseVariable(std::vector<Token>::const_iterator& itr);
    Number*     parseNumber(std::vector<Token>::const_iterator& itr);

    //Next token
    void safe_next(std::vector<Token>::const_iterator& itr) {
        if (itr != end_) {
            ++itr;
        } else {
            throw SyntaxError{ "Premature end of input!" };
        }
    }

    //Return next token's tag without consuming
    int peekTag(std::vector<Token>::const_iterator& itr) {
        auto peek = itr;
        safe_next(peek);        
        if (peek == end_) {
            throw SyntaxError{ "Premature end of input!" };
        }
        return peek->tag;
    }
};

#endif /* Parser_h */
