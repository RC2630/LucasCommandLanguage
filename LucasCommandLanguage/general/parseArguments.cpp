#include "parseArguments.h"

// this is a HELPER function - do NOT call this function outside of this file (parseArguments.h)
string parse::parseArgumentHelper(const string& command, char sep) {
    int indexSep = command.find(string() + sep);
    string argumentPortion = command.substr(indexSep + 1);
    return argumentPortion;
}

// parses the position-th argument of the command
string parse::parseArgument(const string& command, int position, char sep) {
    if (position > numArguments(command, sep)) {
        throw invalid_argument("position (" + to_string(position) + ") > # of arguments (" + to_string(numArguments(command, sep)) + ")");
    }
    if (position == 0) {
        return getCommandName(command, sep);
    }
    string processedCommand = removeChars(command, sep, position - 1);
    string argument = parseArgumentHelper(processedCommand, sep);
    return removeAllAfterChar(argument, sep);
}

// parses the position-th numerical argument of the command
// do NOT use to parse non-numerical arguments
double parse::parseNumericalArgument(const string& command, int position, char sep) {
    return stod(parseArgument(command, position, sep));
}

// parses the position-th boolean argument of the command
// do NOT use to parse non-boolean arguments
bool parse::parseBooleanArgument(const string& command, int position, char sep) {
    return parseBool(parseArgument(command, position, sep));
}

// parses all arguments from the position-th until the end, treating it all as a single argument and returning it
// example: command = "/something arg1 arg2 arg3 arg4 arg5"
// parse 1 until the end returns "arg1 arg2 arg3 arg4 arg5"
// parse 3 until the end returns "arg3 arg4 arg5"
string parse::parseArgumentUntilEnd(const string& command, int position, char sep) {
    if (position > numArguments(command, sep)) {
        throw invalid_argument("position (" + to_string(position) + ") > # of arguments (" + to_string(numArguments(command, sep)) + ")");
    }
    if (position == 0) {
        return command;
    }
    string processedCommand = removeChars(command, sep, position - 1);
    string argument = parseArgumentHelper(processedCommand, sep);
    return argument;
}

// returns the number of arguments in the command
int parse::numArguments(const string& command, char sep) {
    return numOccurrences(command, sep);
}

// checks if the command's name is commandName
bool parse::commandIs(const string& command, const string& commandName, char sep) {
    if (numArguments(command, sep) == 0) {
        return command == commandName;
    } else {
        return beginsWith(command, commandName + sep);
    }
}

// returns a trimmed version of the command consisting only of the command name and the first n arguments
string parse::commandPlusNargs(const string& command, int n, char sep) {
    if (n > numArguments(command, sep)) {
        throw invalid_argument("n (" + to_string(n) + ") > # of arguments (" + to_string(numArguments(command, sep)) + ")");
    }
    if (n == 0) {
        return getCommandName(command, sep);
    }
    string newString;
    int sepCount = 0;
    for (int i = 0; i < command.size(); i++) {
        if (command.at(i) != sep) {
            newString += command.at(i);
        } else {
            sepCount++;
            if (sepCount > n) {
                break;
            } else {
                newString += sep;
            }
        }
    }
    return newString;
}

// returns the command's name
string parse::getCommandName(const string& command, char sep) {
    return strUtil::removeAllAfterChar(command, sep);
}

// parses all arguments of the command, returning a list of arguments
vector<string> parse::parseAllArguments(const string& command, bool includeCommandName, char sep) {
    vector<string> arguments;
    int start = includeCommandName ? 0 : 1;
    for (int i = start; i <= numArguments(command, sep); i++) {
        arguments.push_back(parseArgument(command, i, sep));
    }
    return arguments;
}

// parses all numerical arguments of the command, returning a list of arguments
// note that if includeCommandName is true, then the "command name" needs to be numerical as well (i.e. not a traditional command name)
vector<double> parse::parseAllNumericalArguments(const string& command, bool includeCommandName, char sep) {
    vector<double> arguments;
    int start = includeCommandName ? 0 : 1;
    for (int i = start; i <= numArguments(command, sep); i++) {
        arguments.push_back(parseNumericalArgument(command, i, sep));
    }
    return arguments;
}

// parses all boolean arguments of the command, returning a list of arguments
// note that if includeCommandName is true, then the "command name" needs to be boolean as well (i.e. not a traditional command name)
vector<bool> parse::parseAllBooleanArguments(const string& command, bool includeCommandName, char sep) {
    vector<bool> arguments;
    int start = includeCommandName ? 0 : 1;
    for (int i = start; i <= numArguments(command, sep); i++) {
        arguments.push_back(parseBooleanArgument(command, i, sep));
    }
    return arguments;
}