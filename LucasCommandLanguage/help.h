#ifndef HELP_DOT_H
#define HELP_DOT_H

#include <string>

using namespace std;

// all help messages are presented here as if they are being displayed in the program

namespace help {

	const string HELP_DIRECTORY =

		"LIST OF HELP COMMANDS:\n\n"
		
		"/help = display help directory (this document)\n"
		"/help commands = display help directory for commands (they are grouped into categories such as general, variable, arithmetic, etc.)\n"
		"/help variables = display help document for variables (how to use variables, including examples)\n"
		"/help lclinfo = display information on this programming language (LCL = Lucas Command Language), including copyright";

	const string COMMAND_HELP_DIRECTORY =

		"LIST OF HELP COMMANDS TO FOR ALL CATEGORIES OF COMMANDS:\n\n"

		"/help commands general = display list of general commands\n"
		"/help commands variables = display commands related to variables\n"
		"/help commands arithmetic = display commands related to arithmetic";

	const string COMMAND_HELP_GENERAL =

		"LIST OF GENERAL COMMANDS:\n\n"

		"<empty line> = absolutely nothing (just to make source code look nicer)\n"
		"<any non-empty line that doesn't start with \"/\"> = display the line verbatim\n\n"

		"/stop = terminate program\n"
		"/help = display help directory\n"
		"(for details on specific help commands, type /help)\n"
		"// <anything> = just a comment (the space between // and comment is not required, but it looks nicer with the space)\n"
		"/prev = reuse previous command\n\n"

		"/line = print 1 newline character\n"
		"/line <n> = print n copies of the newline character\n"
		"/digits <n> = use n decimal places for the display of all numerical types (0 <= n <= 12), note that calculations still use much higher precision than display though\n\n"

		"/warntype <b> = set whether or not changing the type of a variable will display a warning message\n"
		"/useblue <b> = set whether or not displaying verbatim lines will be in blue (if false, then it will be in the default colour)\n"
		"/reusedisp <b> = set whether or not /prev will print reuse message whenever it is used";

	const string COMMAND_HELP_VARIABLES =

		"LIST OF VARIABLE-RELATED COMMANDS:\n\n"

		"/store <varname> <value> <type> = store value in a variable called varname with datatype type (Number, Bool, or String)\n"
		"/store <varname> <value> = update value in existing variable called varname, new value must be same type as old value\n"
		"/printvar <varname> = display variable information\n"
		"/varval <varname> = display only variable's value\n\n"

		"Type \"/help variables\" for more details on how to use variables";

	const string COMMAND_HELP_ARITHMETIC =

		"LIST OF ARITHMETIC COMMANDS:\n\n"

		"/add <varname> <n1> <n2> ... <ni> = evaluate n1 + n2 + ... + ni, then store the result into variable called varname\n"
		"/sub <varname> <n1> <n2> ... <ni> = evaluate n1 - n2 - ... - ni, then store the result into variable called varname\n"
		"/times <varname> <n1> <n2> ... <ni> = evaluate n1 * n2 * ... * ni, then store the result into variable called varname\n"
		"/div <varname> <n1> <n2> ... <ni> = evaluate n1 / n2 / ... / ni, then store the result into variable called varname (this is regular division)\n"
		"/fdiv <varname> <n1> <n2> ... <ni> = evaluate n1 / n2 / ... / ni, then store the result into variable called varname (this is floor division)\n"
		"/mod <varname> <n1> <n2> ... <ni> = evaluate n1 % n2 % ... % ni, then store the result into variable called varname\n"
		"/round <varname> <n> = round n to the nearest integer, then store the result into variable called varname";

	const string VARIABLE_HELP =
		
		"HELP WITH VARIABLES:\n\n"

		"Variables can be one of three types (Number, String, Bool) and can be named anything alpha-numeric (including underscores).\n"
		"For the purposes of this document:\n"
		"Variables of type Number are called numerical variables, variables of type String are called string variables, and variables of type Bool are called boolean variables.\n\n"

		"String variables do not have surrounding quotes.\n"
		"Right now, Strings with spaces in them are not supported - your String must not have any spaces.\n"
		"Boolean variables must be either \"true\" or \"false\" (without the quotes).\n"
		"Variables can store commands as well - simply store the command as a String. The no-space rule still applies, though.\n\n"

		"To use variables in your command, there are 2 methods that behave differently for numerical variables:\n"
		"(Note that the 2 methods behave identically for string and boolean variables)\n\n"

		"Method 1: type { followed by the variable's name, then } (ex. type {var} to access the variable var).\n"
		"This method will replace the variable with its rounded value to the number of decimal places set using /digits (3 by default).\n"
		"This is most optimally used for displaying values of variables.\n"
		"Example usage:\n"
		"I go to school {n} times a week!\n\n"
		
		"Method 2: type [ followed by the variable's name, then ] (ex. type [var] to access the variable var).\n"
		"This method will replace the variable with its exact, full-precision value, regardless of /digits.\n"
		"This is most optimally used for performing arithmetic with numerical variables.\n"
		"Example usage:\n"
		"/add sum [n1] [n2] [n3]\n\n"
		
		"Type \"/help commands variables\" for a list of commands that create, modify, and access variables";

	const string LCL_INFO =

		"INFORMATION ON LCL (LUCAS COMMAND LANGUAGE):\n\n"

		"Lucas Command Language (LCL) is the name of this programming language.\n"
		"It was created by Lucas Qin, a computer science student at UBC, in 2021.\n"
		"This language is interpreted, and is written in C++.\n"
		"There are frequent updates to the language as new features are added. Stay tuned!\n\n"

		"The overarching goal is to create a simple, command-based language that is powerful enough to be able to achieve what most other programming languages can do.\n"
		"Obviously, this is still a work in progress, as many key features are still not yet implemented.\n"
		"Due to this language being command-based, a lot of the syntax may look strange or unfamiliar to users of more conventional programming languages.\n"
		"That is perfectly fine - you will learn, you will improve, and you will master LCL very soon!\n\n"

		"Some quick copyright information:\n"
		"(c) Copyright - Lucas Qin, 2021\n"
		"Do not distribute the source code for LCL's interpreter without the permission of Lucas Qin.";

}

#endif