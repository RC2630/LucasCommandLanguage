#ifndef PARSE_DOT_H
#define PARSE_DOT_H

#include "stringUtility.h"

using namespace std;
using namespace strUtil;

namespace parse {

    // this is a HELPER function - do NOT call this function outside of this file (parseArguments.h)
    string parseArgumentHelper(const string& command);

    // parses the position-th argument of the command
    string parseArgument(const string& command, int position = 1);

    // parses the position-th numerical argument of the command
    // do NOT use to parse non-numerical arguments
    double parseNumericalArgument(const string& command, int position = 1);

    // parses the position-th boolean argument of the command
    // do NOT use to parse non-boolean arguments
    bool parseBooleanArgument(const string& command, int position = 1);

    // parses all arguments from the position-th until the end, treating it all as a single argument and returning it
    // example: command = "/something arg1 arg2 arg3 arg4 arg5"
    // parse 1 until the end returns "arg1 arg2 arg3 arg4 arg5"
    // parse 3 until the end returns "arg3 arg4 arg5"
    string parseArgumentUntilEnd(const string& command, int position = 1);

    int numArguments(const string& command);
    bool commandIs(const string& command, const string& commandName);
    // returns a trimmed version of the command consisting only of the command name and the first n arguments
    string commandPlusNargs(const string& command, int n);

}

#endif