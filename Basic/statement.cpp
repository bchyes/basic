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

using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
    /* Empty */
}

Statement::~Statement() {
    /* Empty */
}

/*NumberedState::NumberedState(int token) {
    this->num = token;
}

void NumberedState::execute(EvalState &state) {

}*/

WordState::WordState(string token,int num,string line) {
    if (num)
        //program.addSourceLine(num,line);
    this->op = token;
    this->infor=line;
}
void WordState::execute(EvalState &state) {
    /*if (op == "QUIT") {
        exit(0);
    }
    else if (op == "HELP") {

    }
    else if (op == "CLEAR") {
        program.clear();
    }
    else if (op=="RUN"){
        program.Run(state);
    }
    else if (op=="LIST"){
        program.ListLine();
    }
    else if (op=="REM"){
        //cout<<infor<<"\n";
    }
    else if (op=="LET"){

    }
    else if (op=="PRINT"){

    }
    else if (op=="INPUT"){

    }
    else if (op=="END"){

    }
    else if (op=="GOTO"){

    }
    else if (op=="IF"){

    }*/
}