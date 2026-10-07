//
//  Syntax.cpp
//  lispInterpreter
//
//  Created by Emanuele Ferrando
//

#include "Syntax.h"
#include "Visitor.h"

void Program::accept(Visitor& visitor) const { visitor.visit(*this); }
void Block::accept(Visitor& visitor) const { visitor.visit(*this); }

void Variable::accept(Visitor& visitor) const { visitor.visit(*this); }
void Number::accept(Visitor& visitor) const { visitor.visit(*this); }
void Operator::accept(Visitor& visitor) const { visitor.visit(*this); }

void BoolConst::accept(Visitor& visitor) const { visitor.visit(*this); }
void RelOp::accept(Visitor& visitor) const { visitor.visit(*this); }
void BoolOp::accept(Visitor& visitor) const { visitor.visit(*this); }

void SetStmt::accept(Visitor& visitor) const { visitor.visit(*this); }
void PrintStmt::accept(Visitor& visitor) const { visitor.visit(*this); }
void InputStmt::accept(Visitor& visitor) const { visitor.visit(*this); }
void IfStmt::accept(Visitor& visitor) const { visitor.visit(*this); }
void WhileStmt::accept(Visitor& visitor) const { visitor.visit(*this); }
