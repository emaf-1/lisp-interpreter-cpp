//
//  Parser.cpp
//  lispInterpreter
//
//  Created by Emanuele Ferrando
//

#include <sstream>
#include <cstdint>
#include "Parser.h"

//Stmt_block must start with "(" and a BLOCK (or another statement)
Block* Parser::parseStmtBlock(std::vector<Token>::const_iterator& itr) {
    if (itr == end_) {
        throw SyntaxError{ "Unexpected end of input. Expected '(' instead." };
    }
    if (itr->tag != Token::LP) {
        std::stringstream temp;
        temp << "Missing left parenthesis at token '" << itr->tag << ";" << itr->word << "'";
        throw SyntaxError{ temp.str() };
    }

    if (peekTag(itr) == Token::BLOCK) {
        safe_next(itr); // (
        safe_next(itr); // BLOCK

        Block* b = new Block{};
        while (itr != end_ && itr->tag != Token::RP) {
            b->statements.push_back(parseStatement(itr));
        }
        if (itr == end_) {
            throw SyntaxError{ "Overflow in token stream." };
        }
        safe_next(itr); // )
        if (b->statements.empty()) {
            throw SyntaxError{ "Empty BLOCK statement" };
        }
        return b;
    }
    else {
        Block* b = new Block{};
        b->statements.push_back(parseStatement(itr));
        return b;
    }
}

//Statement: variable_stmt || io_stmt || cond_stmt || loop_stmt
Statement* Parser::parseStatement(std::vector<Token>::const_iterator& itr) {
    if (itr == end_) {
        throw SyntaxError{ "Unexpected end of input. Expected '(' instead." };
    }
    if (itr->tag != Token::LP) {
        std::stringstream temp;
        temp << "Missing left parenthesis at token '" << itr->tag << ";" << itr->word << "'";
        throw SyntaxError{ temp.str() };
    }

    int opTag = peekTag(itr);
    switch (opTag) {
        case Token::SET:   return parseSetStmt(itr);
        case Token::PRINT: return parsePrintStmt(itr);
        case Token::INPUT: return parseInputStmt(itr);
        case Token::IF:    return parseIfStmt(itr);
        case Token::WHILE: return parseWhileStmt(itr);
        default: {
            std::stringstream temp;
            temp << "Unrecognized statement " << Token::id2word[opTag];
            throw SyntaxError{ temp.str() };
        }
    }
}

//Set_stmt
SetStmt* Parser::parseSetStmt(std::vector<Token>::const_iterator& itr) {
    safe_next(itr); // (
    safe_next(itr); // SET

    Variable* v = parseVariable(itr);
    NumExpr* e = parseNumExpr(itr);

    if (itr == end_) {
        throw SyntaxError{ "Unexpected end of input. Expected ')' instead." };
    }
    if (itr->tag != Token::RP) {
        std::stringstream temp;
        temp << "Missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
        throw SyntaxError{ temp.str() };
    }
    safe_next(itr); // )

    return new SetStmt{ v, e };
}

//Print_stmt
PrintStmt* Parser::parsePrintStmt(std::vector<Token>::const_iterator& itr) {
    safe_next(itr); // (
    safe_next(itr); // PRINT

    NumExpr* e = parseNumExpr(itr);

    if (itr == end_) {
        throw SyntaxError{ "Unexpected end of input. Expected ')' instead." };
    }
    
    if (itr->tag != Token::RP) {
        std::stringstream temp;
        temp << "Missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
        throw SyntaxError{ temp.str() };
    }
    safe_next(itr); // )

    return new PrintStmt{ e };
}

//Input_stmt
InputStmt* Parser::parseInputStmt(std::vector<Token>::const_iterator& itr) {
    safe_next(itr); // (
    safe_next(itr); // INPUT

    Variable* v = parseVariable(itr);

    if (itr == end_) {
        throw SyntaxError{ "Unexpected end of input. Expected ')' instead." };
    }
    if (itr->tag != Token::RP) {
        std::stringstream temp;
        temp << "Missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
        throw SyntaxError{ temp.str() };
    }
    safe_next(itr); // )

    return new InputStmt{ v };
}

//If
IfStmt* Parser::parseIfStmt(std::vector<Token>::const_iterator& itr) {
    safe_next(itr); // (
    safe_next(itr); // IF

    BoolExpr* cond = parseBoolExpr(itr);
    Block* thenBlock = parseStmtBlock(itr);
    Block* elseBlock = parseStmtBlock(itr);

    if (itr == end_) {
        throw SyntaxError{ "Unexpected end of input. Expected ')' instead." };
    }
    if (itr->tag != Token::RP) {
        std::stringstream temp;
        temp << "Missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
        throw SyntaxError{ temp.str() };
    }
    safe_next(itr); // )

    return new IfStmt{ cond, thenBlock, elseBlock };
}

//While
WhileStmt* Parser::parseWhileStmt(std::vector<Token>::const_iterator& itr) {
    safe_next(itr); // (
    safe_next(itr); // WHILE

    BoolExpr* cond = parseBoolExpr(itr);
    Block* body = parseStmtBlock(itr);

    if (itr == end_) {
        throw SyntaxError{ "Unexpected end of input. Expected ')' instead." };
    }
    if (itr->tag != Token::RP) {
        std::stringstream temp;
        temp << "Missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
        throw SyntaxError{ temp.str() };
    }
    safe_next(itr); // )

    return new WhileStmt{ cond, body };
}

//ADD/SUB/MUL/DIV
inline bool isArithOp(int tag) {
    return tag == Token::ADD || tag == Token::SUB || tag == Token::MUL || tag == Token::DIV;
}

NumExpr* Parser::parseNumExpr(std::vector<Token>::const_iterator& itr) {
    if (itr == end_) {
            throw SyntaxError{ "Unexpected end of input. Expected NUMBER, VARIABLE_ID or '(' instead." };
        }
        if (itr->tag == Token::LP) {
            int opTag = peekTag(itr);
            if (!isArithOp(opTag)) {
                std::stringstream temp;
                temp << "Unrecognized operator " << Token::id2word[opTag];
                throw SyntaxError{ temp.str() };
            }   
        safe_next(itr); // (
        safe_next(itr); // operator

        NumExpr* left = parseNumExpr(itr);
        NumExpr* right = parseNumExpr(itr);
            
            if (itr == end_) {
                   throw SyntaxError{ "Unexpected end of input. Expected ')' instead." };
               }
            if (itr->tag != Token::RP) {
                std::stringstream temp;
                temp << "Missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
                throw SyntaxError{ temp.str() };
            }
        safe_next(itr); // )

        return new Operator{ opTag, left, right };
    }
    else if (itr->tag == Token::NUMBER) {
        return parseNumber(itr);
    }
    else if (itr->tag == Token::VARIABLE_ID) {
        return parseVariable(itr);
    }
    else {
        std::stringstream temp;
        temp << "Cannot parse expression at token " << itr->word;
        throw SyntaxError{ temp.str() };
    }
}

// LT/GT/EQ || AND/OR || NOT || TRUE/FALSE
BoolExpr* Parser::parseBoolExpr(std::vector<Token>::const_iterator& itr) {
    if (itr == end_) {
        throw SyntaxError{ "Unexpected end of input. Expected TRUE, FALSE or '(' instead." };
    }
    if (itr->tag == Token::LP) {
        int opTag = peekTag(itr);

        if (opTag == Token::GT || opTag == Token::LT || opTag == Token::EQ) {
            safe_next(itr); // (
            safe_next(itr); // operator

            NumExpr* left = parseNumExpr(itr);
            NumExpr* right = parseNumExpr(itr);

            if (itr == end_) {
                throw SyntaxError{ "Unexpected end of input. Expected ')' instead." };
            }
            if (itr->tag != Token::RP) {
                std::stringstream temp;
                temp << "Missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
                throw SyntaxError{ temp.str() };
            }
            safe_next(itr); // )

            return new RelOp{ opTag, left, right };
        }
        else if (opTag == Token::AND || opTag == Token::OR) {
            safe_next(itr); // (
            safe_next(itr); // operator

            BoolExpr* left = parseBoolExpr(itr);
            BoolExpr* right = parseBoolExpr(itr);

            if (itr == end_) {
                throw SyntaxError{ "Unexpected end of input. Expected ')' instead." };
            }
            if (itr->tag != Token::RP) {
                std::stringstream temp;
                temp << "Missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
                throw SyntaxError{ temp.str() };
            }
            safe_next(itr); // )

            return new BoolOp{ opTag, left, right };
        }
        else if (opTag == Token::NOT) {
            safe_next(itr); // (
            safe_next(itr); // NOT

            BoolExpr* operand = parseBoolExpr(itr);

            if (itr == end_) {
                throw SyntaxError{ "Unexpected end of input. Expected ')' instead." };
            }
            if (itr->tag != Token::RP) {
                std::stringstream temp;
                temp << "Missing right parenthesis at token '" << itr->tag << ";" << itr->word << "'";
                throw SyntaxError{ temp.str() };
            }
            safe_next(itr); // )

            return new BoolOp{ opTag, operand, nullptr };
        }
        else {
            std::stringstream temp;
            temp << "Unrecognized operator " << Token::id2word[opTag];
            throw SyntaxError{ temp.str() };
        }
    }
    else if (itr->tag == Token::TRUE_KW) {
        safe_next(itr);
        return new BoolConst{ true };
    }
    else if (itr->tag == Token::FALSE_KW) {
        safe_next(itr);
        return new BoolConst{ false };
    }
    else {
        std::stringstream temp;
        temp << "Cannot parse expression at token " << itr->word;
        throw SyntaxError{ temp.str() };
    }
}

//Variable_id
Variable* Parser::parseVariable(std::vector<Token>::const_iterator& itr) {
    if (itr == end_) {
        throw SyntaxError{ "Unexpected end of input. Expected a variable." };
    }
    if (itr->tag != Token::VARIABLE_ID) {
        std::stringstream temp;
        temp << "Expected a variable, got '" << itr->tag << ";" << itr->word << "'";
        throw SyntaxError{ temp.str() };
    }
    Variable* v = new Variable{ itr->word };
    safe_next(itr);
    return v;
}

//Number
Number* Parser::parseNumber(std::vector<Token>::const_iterator& itr) {
    if (itr == end_) {
        throw SyntaxError{ "Unexpected end of input. Expected a number." };
    }
    if (itr->tag != Token::NUMBER) {
        std::stringstream temp;
        temp << "Cannot parse expression at token " << itr->word;
        throw SyntaxError{ temp.str() };
    }
    std::stringstream temp;
    temp << itr->word;
    int64_t num;
    temp >> num;
    Number* n = new Number{ num };
    safe_next(itr);
    return n;
}
