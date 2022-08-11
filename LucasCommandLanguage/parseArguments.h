#ifndef PARSE_DOT_H
#define PARSE_DOT_H

#include "stringUtility.h"

using namespace std;
using namespace strUtil;

// for all parse functions, argument 0 refers to the command name itself, and the actual arguments are indexed in a 1-based way

namespace parse {

    // this is a HELPER function - do NOT call this function outside of this file (parseArguments.h)
    string parseArgumentHelper(const string& command, char sep = ' ');

    // parses the position-th argument of the command
    string parseArgument(const string& command, int position = 1, char sep = ' ');

    // parses the position-th numerical argument of the command
    // do NOT use to parse non-numerical arguments
    double parseNumericalArgument(const string& command, int position = 1, char sep = ' ');

    // parses the position-th boolean argument of the command
    // do NOT use to parse non-boolean arguments
    bool parseBooleanArgument(const string& command, int position = 1, char sep = ' ');

    // parses all arguments from the position-th until the end, treating it all as a single argument and returning it
    // example: command = "/something arg1 arg2 arg3 arg4 arg5"
    // parse 1 until the end returns "arg1 arg2 arg3 arg4 arg5"
    // parse 3 until the end returns "arg3 arg4 arg5"
    string parseArgumentUntilEnd(const string& command, int position = 1, char sep = ' ');

    // returns the number of arguments in the command
    int numArguments(const string& command, char sep = ' ');

    // checks if the command's name is commandName
    bool commandIs(const string& command, const string& commandName, char sep = ' ');

    // returns a trimmed version of the command consisting only of the command name and the first n arguments
    string commandPlusNargs(const string& command, int n, char sep = ' ');

    // returns the command's name
    string getCommandName(const string& command, char sep = ' ');

    // parses all arguments of the command, returning a list of arguments
    vector<string> parseAllArguments(const string& command, bool includeCommandName = true, char sep = ' ');

    // parses all numerical arguments of the command, returning a list of arguments
    // note that if includeCommandName is true, then the "command name" needs to be numerical as well (i.e. not a traditional command name)
    vector<double> parseAllNumericalArguments(const string& command, bool includeCommandName = true, char sep = ' ');

    // parses all boolean arguments of the command, returning a list of arguments
    // note that if includeCommandName is true, then the "command name" needs to be boolean as well (i.e. not a traditional command name)
    vector<bool> parseAllBooleanArguments(const string& command, bool includeCommandName = true, char sep = ' ');

}

#endif