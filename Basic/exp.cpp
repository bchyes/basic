/*
 * File: exp.cpp
 * -------------
 * This file implements the Expression class and its subclasses.
 */

#include <string>
#include "../StanfordCPPLib/error.h"
#include "evalstate.h"
#include "exp.h"

#include "../StanfordCPPLib/strlib.h"

using namespace std;

/*
 * Implementation notes: the Expression class
 * ------------------------------------------
 * The Expression class declares no instance variables and needs no code.
 */

Expression::Expression() {
    /* Empty */
}

Expression::~Expression() {
    /* Empty */
}

/*
 * Implementation notes: the ConstantExp subclass
 * ----------------------------------------------
 * The ConstantExp subclass declares a single instance variable that
 * stores the value of the constant.  The eval method doesn't use the
 * value of state but needs it to match the general prototype for eval.
 */

ConstantExp::ConstantExp(int value, bool dec) {
    this->value = value;
    this->decimal = dec;
}

int ConstantExp::eval(EvalState &state) {
    return value;
}

string ConstantExp::toString() {
    return integerToString(value);
}

ExpressionType ConstantExp::getType() {
    return CONSTANT;
}

int ConstantExp::getValue() {
    return value;
}

bool ConstantExp::getDecimal() {
    if (decimal) return true;
    return false;
}

/*
 * Implementation notes: the IdentifierExp subclass
 * ------------------------------------------------
 * The IdentifierExp subclass declares a single instance variable that
 * stores the name of the variable.  The implementation of eval must
 * look this variable up in the evaluation state.
 */

IdentifierExp::IdentifierExp(string name) {
    this->name = name;
}

int IdentifierExp::eval(EvalState &state) {
    if (!state.isDefined(name)) {
        identifier_defined = 1;
        return 0;
    }
    return state.getValue(name);
}

string IdentifierExp::toString() {
    return name;
}

ExpressionType IdentifierExp::getType() {
    return IDENTIFIER;
}

string IdentifierExp::getName() {
    return name;
}

bool IdentifierExp::getDefined() {
    return identifier_defined;
}

/*
 * Implementation notes: the CompoundExp subclass
 * ----------------------------------------------
 * The CompoundExp subclass declares instance variables for the operator
 * and the left and right subexpressions.  The implementation of eval 
 * evaluates the subexpressions recursively and then applies the operator.
 */

CompoundExp::CompoundExp(string op, Expression *lhs, Expression *rhs) {
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
}

CompoundExp::~CompoundExp() {
    delete lhs;
    delete rhs;
}

/*
 * Implementation notes: eval
 * --------------------------
 * The eval method for the compound expression case must check for the
 * assignment operator as a special case.  Unlike the arithmetic operators
 * the assignment operator does not evaluate its left operand.
 */

int CompoundExp::eval(EvalState &state) {
    if (op == "<") {
        int left_val = lhs->eval(state);
        int right_val = rhs->eval(state);
        if (left_val < right_val) return 1;
        else return 0;
    }
    if (op == ">") {
        int left_val = lhs->eval(state);
        int right_val = rhs->eval(state);
        if (left_val > right_val) return 1;
        else return 0;
    }
    if (op == "=") {
        if (if_defined){
            int left_val = lhs->eval(state);
            int right_val = rhs->eval(state);
            if (left_val == right_val) return 1;
            else return 0;
        }
        if (lhs->getType() != IDENTIFIER) {
            error("Illegal variable in assignment");
        }
        if (((IdentifierExp *) lhs)->getDefined()) {
            this->compound_defined = 1;
            return 0;
        }
        int val = rhs->eval(state);
        if (!val) {
            ExpressionType type = rhs->getType();
            if (type == COMPOUND)
                if (((CompoundExp *) rhs)->getDefined()) {
                    this->compound_defined = 1;
                    return 0;
                }
        }
        string name = ((IdentifierExp *) lhs)->getName();
        if (name == "LET" || name == "GOTO" || name == "REM" || name == "PRINT" || name == "QUIT" || name == "IF" ||
            name == "END" || name == "INPUT" || name == "LIST" || name == "RUN" || name == "CLEAR" || name == "HELP")
            cout << "SYNTAX ERROR" << "\n";
        else
            state.setValue(name, val);
        return val;
    }
    int left = lhs->eval(state);
    ExpressionType type = lhs->getType();
    if (type == IDENTIFIER) {
        bool left_defined = ((IdentifierExp *) lhs)->getDefined();
        if (left_defined) {
            this->compound_defined = 1;
            return 0;
        }
    }
    int right = rhs->eval(state);
    type = rhs->getType();
    if (type == IDENTIFIER) {
        bool right_defined = ((IdentifierExp *) rhs)->getDefined();
        if (right_defined) {
            this->compound_defined = 1;
            return 0;
        }
    }
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") {
        if (!right) {
            divided = 1;
            return 0;
        }
        return left / right;
    }
    error("Illegal operator in expression");
    return 0;
}

string CompoundExp::toString() {
    return '(' + lhs->toString() + ' ' + op + ' ' + rhs->toString() + ')';
}

ExpressionType CompoundExp::getType() {
    return COMPOUND;
}

string CompoundExp::getOp() {
    return op;
}

Expression *CompoundExp::getLHS() {
    return lhs;
}

Expression *CompoundExp::getRHS() {
    return rhs;
}

bool CompoundExp::getDivided() {
    if (divided) return true;
    return false;
}

bool CompoundExp::getDefined() {
    if (compound_defined) return true;
    return false;
}

void CompoundExp::modifyIf(){
    if_defined=1;
}

