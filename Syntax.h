//
//  Syntax.h
//  lispInterpreter
//
//  Created by Emanuele Ferrando
//

#ifndef Syntax_h
#define Syntax_h

#include <vector>
#include <string>
#include <cstdint>

// Composite/interpreter hierarchy for program syntax according to the CF grammar:
//
// P ::= A | B stands for
// P ::= A
// P ::= B
//
// program          ::= stmt_block
// stmt_block       ::= statement | ( BLOCK statement_list )
// statement_list   ::= statement statement_list | statement
// statement        ::= variable_def | io_stmt | cond_stmt | loop_stmt
// variable_def     ::= ( SET variable_id num_expr )
// io_stmt          ::= ( PRINT num_expr ) | ( INPUT variable_id )
// cond_stmt        ::= ( IF bool_expr stmt_block stmt_block )
// loop_stmt        ::= ( WHILE bool_expr stmt_block )
// num_expr         ::= ( ADD num_expr num_expr ) | ( SUB num_expr num_expr ) | ( MUL num_expr num_expr ) | ( DIV num_expr num_expr ) | number | variable_id
// bool_expr        ::= ( LT num_expr num_expr ) | ( GT num_expr num_expr ) | ( EQ num_expr num_expr ) | ( AND bool_expr bool_expr ) | ( OR bool_expr bool_expr ) | ( NOT bool_expr ) | TRUE | FALSE
//

//Forward declaration
class Visitor;

//Base struct for all statements ast nodes
struct Statement {
    virtual void accept(Visitor& visitor) const = 0;
    virtual ~Statement() = default;
};

//Base struct for all numerical expressions ast nodes
struct NumExpr {
    virtual void accept(Visitor& visitor) const = 0;
    virtual ~NumExpr() = default;
};

//Base struct for all boolean expressions ast nodes
struct BoolExpr {
    virtual void accept(Visitor& visitor) const = 0;
    virtual ~BoolExpr() = default;
};

//AST node representing a block containing a sequence of statements
struct Block {
    void accept(Visitor& visitor) const;
    std::vector<Statement*> statements;     //Sequence of statements
};

//Entire executable program
struct Program {
    Program(Block* b) : block{ b } { }
    void accept(Visitor& visitor) const;
    Block* block;
};

//Variable reference in numerical expressions
struct Variable : public NumExpr {
    Variable(std::string id) : id_{ id } { }
    void accept(Visitor& visitor) const override;
    std::string id_;
};

//64-bit signed integer literal
struct Number : public NumExpr {
    Number(int64_t num) : num_{ num } { }
    void accept(Visitor& visitor) const override;
    int64_t num_;
};

//Binary arithmetic operation
struct Operator : public NumExpr {
    // opCode is one between Token::ADD, Token::SUB, Token::MUL, Token::DIV
    Operator(int opCode, NumExpr* l, NumExpr* r) :
        opCode_{ opCode }, left_{ l }, right_{ r } { }
    void accept(Visitor& visitor) const override;
    int opCode_;
    NumExpr* left_;
    NumExpr* right_;
};

//Bool_expr TRUE/FALSE
struct BoolConst : public BoolExpr {
    BoolConst(bool value) : value_{ value } { }
    void accept(Visitor& visitor) const override;
    bool value_;
};

//Relational operation
struct RelOp : public BoolExpr {
    // opCode is one between Token::GT, Token::LT, Token::EQ
    RelOp(int opCode, NumExpr* l, NumExpr* r) :
        opCode_{ opCode }, left_{ l }, right_{ r } { }
    void accept(Visitor& visitor) const override;
    int opCode_;
    NumExpr* left_;
    NumExpr* right_;
};

//Logical operation
struct BoolOp : public BoolExpr {
    // opCode is one from Token::AND, Token::OR, Token::NOT (right_ remains nullptr)
    BoolOp(int opCode, BoolExpr* l, BoolExpr* r = nullptr) :
        opCode_{ opCode }, left_{ l }, right_{ r } { }
    void accept(Visitor& visitor) const override;
    int opCode_;
    BoolExpr* left_;
    BoolExpr* right_;
};

//Statements
//Assignement
struct SetStmt : public Statement {
    SetStmt(Variable* v, NumExpr* e) : variable_{ v }, expr_{ e } { }
    void accept(Visitor& visitor) const override;
    Variable* variable_;
    NumExpr* expr_;
};

//Printing
struct PrintStmt : public Statement {
    PrintStmt(NumExpr* e) : expr_{ e } { }
    void accept(Visitor& visitor) const override;
    NumExpr* expr_;
};

//Input reading
struct InputStmt : public Statement {
    InputStmt(Variable* v) : variable_{ v } { }
    void accept(Visitor& visitor) const override;
    Variable* variable_;
};

//Logic condition if
struct IfStmt : public Statement {
    IfStmt(BoolExpr* c, Block* t, Block* e) :
        cond_{ c }, thenBlock_{ t }, elseBlock_{ e } { }
    void accept(Visitor& visitor) const override;
    BoolExpr* cond_;
    Block* thenBlock_;
    Block* elseBlock_;
};

//Logic condition while
struct WhileStmt : public Statement {
    WhileStmt(BoolExpr* c, Block* b) : cond_{ c }, body_{ b } { }
    void accept(Visitor& visitor) const override;
    BoolExpr* cond_;
    Block* body_;
};

#endif /* Syntax_h */
