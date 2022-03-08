#include "parseArguments.h"

// this is a HELPER function - do NOT call this function outside of this file (parseArguments.h)
string parse::parseArgumentHelper(const string& command) {
    int indexSpace = command.find(" ");
    string argumentPortion = command.substr(indexSpace + 1);
    return argumentPortion;
}

// parses the position-th argument of the command
string parse::parseArgument(const string& command, int position) {
    if (position > numArguments(command)) {
        throw invalid_argument("position (" + to_string(position) + ") > # of arguments (" + to_string(numArguments(command)) + ")");
    }
    if (position == 0) {
        return getCommandName(command);
    }
    string processedCommand = removeChars(command, ' ', position - 1);
    string argument = parseArgumentHelper(processedCommand);
    return removeAllAfterChar(argument, ' ');
}

// parses the position-th numerical argument of the command
// do NOT use to parse non-numerical arguments
double parse::parseNumericalArgument(const string& command, int position) {
    return stod(parseArgument(command, position));
}

// parses the position-th boolean argument of the command
// do NOT use to parse non-boolean arguments
bool parse::parseBooleanArgument(const string& command, int position) {
    return parseBool(parseArgument(command, position));
}

// parses all arguments from the position-th until the end, treating it all as a single argument and returning it
// example: command = "/something arg1 arg2 arg3 arg4 arg5"
// parse 1 until the end returns "arg1 arg2 arg3 arg4 arg5"
// parse 3 until the end returns "arg3 arg4 arg5"
string parse::parseArgumentUntilEnd(const string& command, int position) {
    if (position > numArguments(command)) {
        throw invalid_argument("position (" + to_string(position) + ") > # of arguments (" + to_string(numArguments(command)) + ")");
    }
    if (position == 0) {
        return command;
    }
    string processedCommand = removeChars(command, ' ', position - 1);
    string argument = parseArgumentHelper(processedCommand);
    return argument;
}

int parse::numArguments(const string& command) {
    return numOccurrences(command, ' ');
}

bool parse::commandIs(const string& command, const string& commandName) {
    if (numArguments(command) == 0) {
        return command == commandName;
    } else {
        return beginsWith(command, commandName + " ");
    }
}

// returns a trimmed version of the command consisting only of the command name and the first n arguments
string parse::commandPlusNargs(const string& command, int n) {
    if (n > numArguments(command)) {
        throw invalid_argument("n (" + to_string(n) + ") > # of arguments (" + to_string(numArguments(command)) + ")");
    }
    if (n == 0) {
        return getCommandName(command);
    }
    string newString;
    int spaceCount = 0;
    for (int i = 0; i < command.size(); i++) {
        if (command.at(i) != ' ') {
            newString += command.at(i);
        } else {
            spaceCount++;
            if (spaceCount > n) {
                break;
            } else {
                newString += ' ';
            }
        }
    }
    return newString;
}

string parse::getCommandName(const string& command) {
    return strUtil::removeAllAfterChar(command, ' ');
}