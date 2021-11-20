/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
//#include "statement.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"

using namespace std;

/* Function prototypes */

void processLine(string line, Program &program, EvalState &state);

void help();

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            string input = getLine();
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            cerr << "Error: " << ex.getMessage() << endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program &program, EvalState &state) {
    static bool input = 0;
    static string input_string;
    int flag = 1;
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    if (input) {
        if (line[0] == '-') {
            flag = -1;
            line = line.substr(1);
        }
        for (int i = 0; i < line.length(); i++) {
            if (line[i] < '0' || line[i] > '9') {
                cout << "INVALID NUMBER" << "\n";
                cout << " ? ";
                return;
            }
        }
        scanner.setInput(line);
        Expression *exp = parseExp(scanner);
        state.setValue(input_string, flag * (((ConstantExp *) exp)->getValue()));
        flag = 1;
        input = 0;
        return;
    }
    scanner.setInput(line);
    string token = scanner.nextToken();
    TokenType type = scanner.getTokenType(token);
    int numbertaken = 0;
    if (type == NUMBER) {
        numbertaken = stringToInteger(token);
        if (!scanner.hasMoreTokens()) {
            program.removeSourceLine(numbertaken);
            return;
        }
        line = line.substr(token.length() + 1);
        token = scanner.nextToken();
    }
    if (numbertaken)
        program.addSourceLine(numbertaken, line);
    else {
        if (token == "QUIT") {
            exit(0);
        } else if (token == "HELP") {
            help();
        } else if (token == "CLEAR") {
            program.clear();
            state.clear();
        } else if (token == "RUN") {
            program.Run(state);
        } else if (token == "LIST") {
            program.ListLine();
        } else if (token == "LET") {
            Expression *exp = parseExp(scanner);
            exp->eval(state);
            if (exp->getType() == COMPOUND)
                if (((CompoundExp *) exp)->getDefined()) {
                    cout << "VARIABLE NOT DEFINED" << "\n";
                    return;
                }
        } else if (token == "PRINT") {
            Expression *exp = parseExp(scanner);
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
        } else if (token == "INPUT") {
            input_string = scanner.nextToken();
            input = 1;
            cout << " ? ";
        }
    }
}

void help() {
    printf(
    "this interpreter process the operation without line number in the processline\n"
    "and it store the line number and its information in the project which contains a map in it\n"
    "it store the line number as key and information as value and we use iterator to process the every line operation"
    );
}