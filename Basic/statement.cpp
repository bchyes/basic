/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "../StanfordCPPLib/simpio.h"

using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
    /* Empty */
}

Statement::~Statement() {
    /* Empty */
}

Letstate::Letstate(Expression *stmt) : stmt(stmt) {}

void Letstate::execute(EvalState &state) {
    stmt->eval(state);
    if (stmt->getType() == COMPOUND)
        if (((CompoundExp *) stmt)->getDefined()) {
            cout << "VARIABLE NOT DEFINED" << "\n";
        }
}

Stmt Letstate::getType() {
    return Let;
}

Gotostate::Gotostate(Expression *stmt, int line) : stmt(stmt), line(line) {}

void Gotostate::execute(EvalState &state) {
    if (stmt->getType() == CONSTANT)
        line = stmt->eval(state);
}

Stmt Gotostate::getType() {
    return Goto;
}

int Gotostate::getNum() {
    return line;
}

Ifstate::Ifstate(Expression *stmt, Expression *stmt1, int line) : stmt(stmt), stmt_1(stmt1), line(line) {}

void Ifstate::execute(EvalState &state) {
    ((CompoundExp *) stmt)->modifyIf();
    int value = stmt->eval(state);
    if (value) {
        if (stmt_1->getType() == CONSTANT)
            line = value;
    }
}

Stmt Ifstate::getType() {
    return If;
}

int Ifstate::getNum() {
    return line;
}

Printstate::Printstate(Expression *stmt) : stmt(stmt) {}

void Printstate::execute(EvalState &state) {
    int value = stmt->eval(state);
    if (stmt->getType() == IDENTIFIER)
        if (((IdentifierExp *) stmt)->getDefined()) {
            cout << "VARIABLE NOT DEFINED" << "\n";
            return;
        }
    if (!value) {
        if (stmt->getType() == COMPOUND)
            if (((CompoundExp *) stmt)->getDivided()) {
                cout << "DIVIDE BY ZERO" << "\n";
                return;
            }
    }
    cout << value << "\n";
}

Stmt Printstate::getType() {
    return Print;
}

void Inputstate::execute(EvalState &state) {
    string input_num;
    int flag = 1;
    bool input_true = 0;
    input_num = getLine();
    cout << " ? ";
    while (1) {
        if (input_num[0] == '-') {
            flag = -1;
            input_num = input_num.substr(1);
        }
        for (int i = 0; i < input_num.length(); i++) {
            if (input_num[i] < '0' || input_num[i] > '9') {
                cout << "INVALID NUMBER" << "\n";
                cout << " ? ";
                input_true = 1;
                break;
            }
        }
        if (!input_true) break;
        input_true = 0;
    }
    state.setValue(Identifier, flag * stringToInteger(input_num));
}

Stmt Inputstate::getType() {
    return Input;
}

Inputstate::Inputstate(const string &identifier) : Identifier(identifier) {}

void Endstate::execute(EvalState &state) {

}

Stmt Endstate::getType() {
    return End;
}
