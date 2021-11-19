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
    // Replace this stub with your own code
    //for (it=line_information.begin();it!=line_information.end();it++){
    //    line_information.erase(it);
    //}
    line_information.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
    // Replace this stub with your own code
    Information Infor;
    Infor.line = line;
    line_information[lineNumber] = Infor;
}

void Program::removeSourceLine(int lineNumber) {
    // Replace this stub with your own code
    line_information.erase(lineNumber);
}

string Program::getSourceLine(int lineNumber) {
    return line_information[lineNumber].line;    // Replace this stub with your own code
}

//void Program::setParsedStatement(int lineNumber, Statement *stmt) {
//    // Replace this stub with your own code
//}

//Statement *Program::getParsedStatement(int lineNumber) {
//    return NULL;  // Replace this stub with your own code
//}

int Program::getFirstLineNumber() {
    return (line_information.begin())->first;     // Replace this stub with your own code
}

int Program::getNextLineNumber(int lineNumber) {
    return (line_information.lower_bound(lineNumber))->first;     // Replace this stub with your own code
}

void Program::ListLine() {
    for (it = line_information.begin(); it != line_information.end(); it++) {
        cout << it->first << " " << it->second.line << "\n";
    }
}

void Program::Run(EvalState &state) {
    bool go=1;
    for (it = line_information.begin();;) {
        if (go) {
            go=0;
        }
        else it++;
        if (it == line_information.end()) break;
        Information infor = it->second;
        TokenScanner scan;
        scan.ignoreWhitespace();
        scan.scanNumbers();
        scan.setInput(infor.line);
        string op = scan.nextToken();
        if (op == "REM") {
            //infor.line.substr(4);
            //cout<<infor.line<<"\n";
        } else if (op == "LET") {
            Expression *exp = parseExp(scan);
            exp->eval(state);
            if (exp->getType() == COMPOUND)
                if (((CompoundExp *) exp)->getDefined()) {
                    cout << "VARIABLE NOT DEFINED" << "\n";
                    return;
                }
        } else if (op == "PRINT") {
            Expression *exp = parseExp(scan);
            int value = exp->eval(state);
            if (exp->getType() == IDENTIFIER)
                if (((IdentifierExp *) exp)->getDefined()) {
                    cout << "VARIABLE NOT DEFINED" << "\n";
                    return;
                }
            if (!value) {
                if (exp->getType() == COMPOUND)
                    if (((CompoundExp *) exp)->getDivided()) {
                        cout << "DIVIDE BY ZERO" << "\n";
                        return;
                    }
            }
            cout << value << "\n";
        } else if (op == "INPUT") {
            string input_num;
            int flag = 1;
            bool input_true=0;
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
                        input_true=1;
                        break;
                    }
                }
                if (!input_true) break;
                input_true=0;
            }
            state.setValue(scan.nextToken(), flag * stringToInteger(input_num));
        } else if (op == "END") {
            return;
        } else if (op == "GOTO") {
            Expression *exp = parseExp(scan);
            if (exp->getType() == CONSTANT)
                if (!line_information.count(((ConstantExp *) exp)->getValue())) cout << "LINE NUMBER ERROR" << "\n";
                else {
                    it = line_information.find(((ConstantExp *) exp)->getValue());
                    go=1;
                }
        } else if (op == "IF") {
            infor.line=infor.line.substr(op.length()+1);
            for (int i=0;i<infor.line.length();i++)
                if (infor.line[i]=='T'){
                    op=infor.line.substr(0,i-1);
                    infor.line=infor.line.substr(i+5);
                    break;
                }
            TokenScanner scan_1;
            scan_1.ignoreWhitespace();
            scan_1.scanNumbers();
            scan_1.setInput(op);
            Expression *exp = parseExp(scan_1);
            ((CompoundExp *)exp)->modifyIf();
            int value=exp->eval(state);
            if (value){
                scan_1.setInput(infor.line);
                Expression *exp = parseExp(scan_1);
                if (exp->getType() == CONSTANT)
                    if (!line_information.count(((ConstantExp *) exp)->getValue())) cout << "LINE NUMBER ERROR" << "\n";
                    else {
                        it = line_information.find(((ConstantExp *) exp)->getValue());
                        go=1;
                    }
            }

        }
    }
}
