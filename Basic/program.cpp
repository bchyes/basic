/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
#include "parser.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/simpio.h"

using namespace std;

Program::Program() {
    // Replace this stub with your own code
}

Program::~Program() {
    // Replace this stub with your own code
}

void Program::clear() {
    line_information.clear();
    line_parsed.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
    Information Infor;
    Infor.line = line;
    line_information[lineNumber] = Infor;
    line_parsed[lineNumber] = getParsedStatement(lineNumber);
}

void Program::removeSourceLine(int lineNumber) {
    line_information.erase(lineNumber);
    delete line_parsed[lineNumber];
    line_parsed.erase(lineNumber);
}

string Program::getSourceLine(int lineNumber) {
    return line_information[lineNumber].line;
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    line_parsed[lineNumber] = stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
    Information infor = line_information[lineNumber];
    TokenScanner scan;
    scan.ignoreWhitespace();
    scan.scanNumbers();
    scan.setInput(infor.line);
    string op = scan.nextToken();
    if (op == "LET") {
        Expression *exp = parseExp(scan);
        Statement *stmt = new Letstate(exp);
        return stmt;
    } else if (op == "PRINT") {
        Expression *exp = parseExp(scan);
        Statement *stmt = new Printstate(exp);
        return stmt;
    } else if (op == "INPUT") {
        string Iden = scan.nextToken();
        Statement *stmt = new Inputstate(Iden);
        return stmt;
    } else if (op == "END") {
        Statement *stmt = new Endstate();
        return stmt;
    } else if (op == "GOTO") {
        Expression *exp = parseExp(scan);
        Statement *stmt = new Gotostate(exp);
        return stmt;
    } else if (op == "IF") {
        infor.line = infor.line.substr(op.length() + 1);
        for (int i = 0; i < infor.line.length(); i++)
            if (infor.line[i] == 'T') {
                op = infor.line.substr(0, i - 1);
                infor.line = infor.line.substr(i + 5);
                break;
            }
        scan.setInput(op);
        Expression *exp = parseExp(scan);
        scan.setInput(infor.line);
        Expression *exp_1 = parseExp(scan);
        Statement *stmt = new Ifstate(exp, exp_1);
        return stmt;
    }
}

int Program::getFirstLineNumber() {
    return (line_information.begin())->first;
}

int Program::getNextLineNumber(int lineNumber) {
    return (line_information.lower_bound(lineNumber))->first;
}

void Program::ListLine() {
    for (it = line_information.begin(); it != line_information.end(); it++) {
        cout << it->first << " " << it->second.line << "\n";
    }
}

void Program::Run(EvalState &state) {
    bool go = 1;
    it = line_information.begin();
    it_ = line_parsed.begin();
    for (;;) {
        if (go) {
            go = 0;
        } else {
            it++;
            it_++;
        }
        if (it == line_information.end()) break;
        it_->second->execute(state);
        if (it_->second->getType() == End) break;
        else if (it_->second->getType() == Goto) {
            int num = ((Gotostate *) (it_->second))->getNum();
            if (!line_information.count(num)) cout << "LINE NUMBER ERROR" << "\n";
            else {
                it = line_information.find(num);
                it_ = line_parsed.find(num);
                go = 1;
            }
        } else if (it_->second->getType() == If) {
            int num = ((Ifstate *) (it_->second))->getNum();
            if (!line_information.count(num)) cout << "LINE NUMBER ERROR" << "\n";
            else {
                it = line_information.find(num);
                it_ = line_parsed.find(num);
                go = 1;
            }
        }
    }
}
