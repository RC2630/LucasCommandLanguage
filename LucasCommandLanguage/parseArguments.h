#ifndef PARSE_DOT_H
#define PARSE_DOT_H

#include "stringUtility.h"

using namespace std;
using namespace strUtil;

namespace parse {

    string parseArgumentHelper(const string& command) {
        int indexSpace = command.find(" ");
        string argumentPortion = command.substr(indexSpace + 1);
        return argumentPortion;
    }

    // parses the position-th argument of the command
    string parseArgument(const string& command, int position = 1) {
        string processedCommand = removeChars(command, ' ', position - 1);
        string argument = parseArgumentHelper(processedCommand);
        return removeAllAfterChar(argument, ' ');
    }

    // parses the position-th numerical argument of the command
    // do NOT use to parse non-numerical arguments
    double parseNumericalArgument(const string& command, int position = 1) {
        return stod(parseArgument(command, position));
    }

    // parses the position-th boolean argument of the command
    // do NOT use to parse non-boolean arguments
    bool parseBooleanArgument(const string& command, int position = 1) {
        return parseBool(parseArgument(command, position));
    }

    int numArguments(const string& command) {
        return numOccurrences(command, ' ');
    }

    bool commandIs(const string& command, const string& commandName) {
        if (numArguments(command) == 0) {
            return command == commandName;
        } else {
            return beginsWith(command, commandName + " ");
        }
    }

}

#endif