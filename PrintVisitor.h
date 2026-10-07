//
//  PrintVisitor.h
//  lispInterpreter
//
//  Created by Emanuele Ferrando
//

#ifndef PrintVisitor_h
#define PrintVisitor_h

#include <iostream>
#include "Visitor.h"
#include "Syntax.h"
#include "Token.h"

class PrintVisitor : public Visitor {
public:
    PrintVisitor(std::ostream& con) : console_{ con } { }

    //Entry point: visits root block
    void visit(Program const& p) override {
        p.block->accept(*this);
        console_ << std::endl;
    }

    //Prints a block as (BLOCK statements...)
    void visit(Block const& b) override {
        console_ << "(BLOCK ";
        for (Statement* statement : b.statements) {
            statement->accept(*this);
            console_ << " ";
        }
        console_ << ")";
    }

    //Prints variable assignment as (SET variable expression)
    void visit(SetStmt const& s) override {
        console_ << "(SET ";
        s.variable_->accept(*this);
        console_ << " ";
        s.expr_->accept(*this);
        console_ << ")";
    }

    //Prints print statement as (PRINT expression)
    void visit(PrintStmt const& p) override {
        console_ << "(PRINT ";
        p.expr_->accept(*this);
        console_ << ")";
    }

    //Prints input statement as (INPUT variable)
    void visit(InputStmt const& i) override {
        console_ << "(INPUT ";
        i.variable_->accept(*this);
        console_ << ")";
    }

    //Prints conditional statement as (IF condition thenBlock elseBlock)
    void visit(IfStmt const& i) override {
        console_ << "(IF ";
        i.cond_->accept(*this);
        console_ << " ";
        i.thenBlock_->accept(*this);
        console_ << " ";
        i.elseBlock_->accept(*this);
        console_ << ")";
    }
    
    //Prints loop statement as (WHILE condition bodyBlock)
    void visit(WhileStmt const& w) override {
        console_ << "(WHILE ";
        w.cond_->accept(*this);
        console_ << " ";
        w.body_->accept(*this);
        console_ << ")";
    }

    //Prints binary arithmetic operation as (OP left right)
    void visit(Operator const& o) override {
        console_ << Token::id2word[Token::LP];
        console_ << Token::id2word[o.opCode_] << " ";
        o.left_->accept(*this);
        console_ << " ";
        o.right_->accept(*this);
        console_ << Token::id2word[Token::RP];
    }

    //Prints numeric literal
    void visit(Number const& n) override {
        console_ << n.num_;
    }

    //Prints variable id
    void visit(Variable const& v) override {
        console_ << v.id_;
    }
    
    //Prints relational comparison as (REL_OP left right)
    void visit(RelOp const& r) override {
        console_ << Token::id2word[Token::LP];
        console_ << Token::id2word[r.opCode_] << " ";
        r.left_->accept(*this);
        console_ << " ";
        r.right_->accept(*this);
        console_ << Token::id2word[Token::RP];
    }
    
    //Prints logical operation as (LOGIC_OP left [right])
    void visit(BoolOp const& b) override {
        console_ << Token::id2word[Token::LP];
        console_ << Token::id2word[b.opCode_];
        console_ << " ";
        b.left_->accept(*this);
        if (b.right_ != nullptr) {
            console_ << " ";
            b.right_->accept(*this);
        }
        console_ << Token::id2word[Token::RP];
    }
    
    // Prints boolean TRUE/FALSE
    void visit(BoolConst const& b) override {
        console_ << (b.value_ ? Token::id2word[Token::TRUE_KW]
                               : Token::id2word[Token::FALSE_KW]);
    }

private:
    std::ostream& console_;     //Ostream target (console)
};

#endif /* PrintVisitor_h */
