//
//  EvaluationVisitor.h
//  lispInterpreter
//
//  Created by Emanuele Ferrando
//

#ifndef EvaluationVisitor_h
#define EvaluationVisitor_h

#include <iostream>
#include <sstream>
#include <cstdint>

#include "Visitor.h"
#include "Syntax.h"
#include "SymbolTable.h"
#include "Exceptions.h"

//Visitor that evaluates the Abstract Syntax Tree
class EvaluationVisitor : public Visitor {
public:
    EvaluationVisitor(SymbolTable& st, std::istream& in, std::ostream& out) :
        symbolTable_{ st }, input_{ in }, console_{ out } { }
    //Evaluates the root program node by visiting its entry block
    void visit(Program const& p) override {
        p.block->accept(*this);
    }
    //After, evaluates each statement in the block
    void visit(Block const& b) override {
        for (Statement* statement : b.statements) {
            statement->accept(*this);
        }
    }
    //Set -> evaluates the  expression and assigns the result
    void visit(SetStmt const& s) override {
        s.expr_->accept(*this);
        symbolTable_.setValue(s.variable_->id_, numRegister_);
    }
    //Print -> evaluates and writes the numerical result
    void visit(PrintStmt const& p) override {
        p.expr_->accept(*this);
        console_ << numRegister_ << std::endl;
    }
    //Input -> reads an integer value and stores it in the given variable
    void visit(InputStmt const& i) override {
        std::string word;
        input_ >> word;
        if (input_.fail()) {
            throw EvaluationError{ "Invalid input: expected a number." };
        }
        //no '+' sign and no starting zeros (only "0" is valid)
        size_t firstDigit = (word[0] == '-') ? 1 : 0;
        if (word[0] == '+' || (word.size() > firstDigit + 1 && word[firstDigit] == '0')) {
            throw EvaluationError{ "Invalid input: expected a number." };
        }
        //converts the word and rejects anything that is not entirely a number
        std::stringstream temp;
        temp << word;
        int64_t value;
        temp >> value;
        if (temp.fail() || !temp.eof()) {
            throw EvaluationError{ "Invalid input: expected a number." };
        }
        symbolTable_.setValue(i.variable_->id_, value);
    }
    //If -> evaluates branch execution based on the boolean condition
    void visit(IfStmt const& i) override {
        i.cond_->accept(*this);
        if (boolRegister_) {
            i.thenBlock_->accept(*this);
        } else {
            i.elseBlock_->accept(*this);
        }
    }
    //While -> evaluates and executes loop body while condition is true
    void visit(WhileStmt const& w) override {
        w.cond_->accept(*this);
        while (boolRegister_) {
            w.body_->accept(*this);
            w.cond_->accept(*this);
        }
    }
    //Evaluates arithmetic operators (+, -, *, /) and stores result in numRegister_
    void visit(Operator const& o) override {
        o.left_->accept(*this);
        int64_t leftVal = numRegister_;
        o.right_->accept(*this);
        int64_t rightVal = numRegister_;
        //switch with Op code and execution of operations
        switch (o.opCode_) {
            case Token::ADD: numRegister_ = leftVal + rightVal; break;
            case Token::SUB: numRegister_ = leftVal - rightVal; break;
            case Token::MUL: numRegister_ = leftVal * rightVal; break;
            case Token::DIV:
                if (rightVal == 0) {    //"immediate" operations
                    throw EvaluationError{ "Division by zero." };
                }
                if (rightVal == -1) {
                    numRegister_ = -leftVal;
                } else {
                    numRegister_ = leftVal / rightVal;
                }
                break;
            default:
                throw EvaluationError{ "Unknown arithmetic operator." };
        }
    }
    //Loads an integer constant into the num register
    void visit(Number const& n) override {
        numRegister_ = n.num_;
    }
    
    //Looks up variable's current value in the symbol table
    void visit(Variable const& v) override {
        numRegister_ = symbolTable_.getValue(v.id_);
    }
    //As "operators", evaluates operators (>, <, ==) and stores boolean result into the boolRegister_
    void visit(RelOp const& r) override {
        r.left_->accept(*this);
        int64_t leftVal = numRegister_;
        r.right_->accept(*this);
        int64_t rightVal = numRegister_;

        switch (r.opCode_) {
            case Token::GT: boolRegister_ = (leftVal > rightVal); break;
            case Token::LT: boolRegister_ = (leftVal < rightVal); break;
            case Token::EQ: boolRegister_ = (leftVal == rightVal); break;
            default:
                throw EvaluationError{ "Unknown relational operator." };
        }
    }
    //As "operators", evaluates logical operators for boolean. AND/OR skip evaluating the right operand when the result is already determined
    void visit(BoolOp const& b) override {
        b.left_->accept(*this);
        bool leftVal = boolRegister_;
        //NOT
        if (b.opCode_ == Token::NOT) {
            boolRegister_ = !leftVal;
            return;
        }
        //AND
        if (b.opCode_ == Token::AND) {
            if (!leftVal) {
                boolRegister_ = false;
                return;
            }
            b.right_->accept(*this);

            return;
        }
        //OR
        if (b.opCode_ == Token::OR) {
            if (leftVal) {
                boolRegister_ = true;
                return;
            }
            b.right_->accept(*this);
            return;
        }
        
        throw EvaluationError{ "Unknown logical operator." };
    }
    // Loads boolean value into the bool register
    void visit(BoolConst const& b) override {
        boolRegister_ = b.value_;
    }

private:
    SymbolTable& symbolTable_;      //Refers to symboltable
    std::istream& input_;           //Input stream
    std::ostream& console_;         //Output target (console)

    int64_t numRegister_{ 0 };      //Register for intermediate num operations
    bool boolRegister_{ false };    //Register for intermediate logical operations
};

#endif /* EvaluationVisitor_h */
