//
//  Visitor.h
//  lispInterpreter
//
//  Created by Emanuele Ferrando
//

#ifndef Visitor_h
#define Visitor_h

#include "Syntax.h"

class Visitor {
public:
    virtual void visit(Program const& p) = 0;
    virtual void visit(Block const& b) = 0;

    //Statements
    virtual void visit(SetStmt const& s) = 0;
    virtual void visit(PrintStmt const& p) = 0;
    virtual void visit(InputStmt const& i) = 0;
    virtual void visit(IfStmt const& i) = 0;
    virtual void visit(WhileStmt const& w) = 0;

    //NumExpr
    virtual void visit(Operator const& o) = 0;
    virtual void visit(Number const& n) = 0;
    virtual void visit(Variable const& v) = 0;

    //BoolExpr
    virtual void visit(RelOp const& r) = 0;
    virtual void visit(BoolOp const& b) = 0;
    virtual void visit(BoolConst const& b) = 0;
};

#endif /* Visitor_h */
