#include "file.h"
#include "parseArguments.h"
#include "vectorUtility.h"
#include "stringUtility.h"
#include "help.h"
#include "variable.h"
#include "ansi_codes.h"
#include "numberUtility.h"

#include <iomanip>
#include <stdexcept>

using namespace std;
using namespace file;
using namespace parse;
using namespace vecUtil;
using namespace strUtil;
using namespace help;
using namespace var;
using namespace numUtil;

const string INPUT_FILE = "../LucasCommandLanguage/inputFile/input.txt";

bool reuse_display = true;
bool warn_type_change = true;
bool use_blue = true;
int num_places = 3;
vector<Variable> vars;

// primary function for interpreting commands
void interpretCommand(const string& command);

// helpers for individual commands
void helpWith1Arg(const string& specific);
void helpWith2Arg(const string& spec1, const string& spec2);
void line(int n);
void createVar(const string& name, const string& val, const string& type);
void updateVar(const string& name, const string& val);
void digits(int n);
void printVar(const string& varname);
void printVarValue(const string& varname);
void digitCommand(int n);

// arithmetic helpers
void add(const string& command);
void sub(const string& command);
void times(const string& command);
void div(const string& command);
void fdiv(const string& command);
void mod(const string& command);
void round(const string& command);

// a testing function that executes when the main program terminates
void test(vector<string>& com);

int main() {

	digits(num_places);
	
	vector<string> commands;
	inputStrVecFrom(commands, INPUT_FILE);
	commands.push_back("/stop");
	
	string command;
	string prevCommand;
	int currIndex = 0;

	cout << "\n";

	if (commands.size() == 1) { // the only command is "/stop" (i.e. the source code is empty)
		cout << ANSI_GREEN << "The source code is empty.\n"
			 << "Type \"/help\" in the source code if you need help getting started with this programmming language.\n\n" << ANSI_NORMAL;
	}

	try {

		while (true) {

			prevCommand = command;
			command = commands.at(currIndex);
			currIndex++;

			replaceVariableReferencesWithRoundedValues(command, vars, num_places);
			replaceVariableReferencesWithFullPrecisionValues(command, vars);

			if (command == "/prev") {
				if (prevCommand == "" || prevCommand == "/prev") {
					cout << ANSI_RED << "Sorry, but there is no previous command.\n" << ANSI_NORMAL;
					continue;
				} else {
					if (reuse_display) {
						cout << ANSI_GREEN << "Reusing previous command \"" << prevCommand << "\" ......\n" << ANSI_NORMAL;
					}
					command = prevCommand;
				}
			}

			if (command.empty()) {
				continue;
			}

			if (!beginsWith(command, "/")) {
				if (use_blue) {
					cout << ANSI_BLUE;
				}
				cout << command << "\n" << ANSI_NORMAL;
				continue;
			}

			if (command == "/stop") {
				cout << ANSI_GREEN << "Program has terminated.\n" << ANSI_NORMAL;
				test(commands);
				break;
			}

			interpretCommand(command);

		}

	} catch (...) {
		cout << ANSI_RED << "Program has crashed unexpectedly.\n"
			 << "The command that crashed it is \"" << command << "\"\n" << ANSI_NORMAL;
	}

}

void interpretCommand(const string& command) {

	if (beginsWith(command, "/help") && numArguments(command) == 0) {
		cout << ANSI_GREEN << HELP_DIRECTORY << "\n" << ANSI_NORMAL;
	} else if (beginsWith(command, "/help") && numArguments(command) == 1) {
		helpWith1Arg(parseArgument(command));
	} else if (beginsWith(command, "/help") && numArguments(command) == 2) {
		helpWith2Arg(parseArgument(command, 1), parseArgument(command, 2));
	} else if (beginsWith(command, "//")) {
		// ignore this command because it is a comment
	} else if (beginsWith(command, "/line") && numArguments(command) == 0) {
		line(1);
	} else if (beginsWith(command, "/line") && numArguments(command) == 1) {
		line(parseNumericalArgument(command));
	} else if (beginsWith(command, "/reusedisp")) {
		reuse_display = parseBooleanArgument(command);
	} else if (beginsWith(command, "/warntype")) {
		warn_type_change = parseBooleanArgument(command);
	} else if (beginsWith(command, "/useblue")) {
		use_blue = parseBooleanArgument(command);
	} else if (beginsWith(command, "/store") && numArguments(command) == 3) {
		createVar(parseArgument(command, 1), parseArgument(command, 2), parseArgument(command, 3));
	} else if (beginsWith(command, "/store") && numArguments(command) == 2) {
		updateVar(parseArgument(command, 1), parseArgument(command, 2));
	} else if (beginsWith(command, "/digits")) {
		digitCommand(parseNumericalArgument(command));
	} else if (beginsWith(command, "/printvar")) {
		printVar(parseArgument(command));
	} else if (beginsWith(command, "/varval")) {
		printVarValue(parseArgument(command));
	} else if (beginsWith(command, "/add")) {
		add(command);
	} else if (beginsWith(command, "/sub")) {
		sub(command);
	} else if (beginsWith(command, "/times")) {
		times(command);
	} else if (beginsWith(command, "/div")) {
		div(command);
	} else if (beginsWith(command, "/fdiv")) {
		fdiv(command);
	} else if (beginsWith(command, "/mod")) {
		mod(command);
	} else if (beginsWith(command, "/round")) {
		round(command);
	} else {
		cout << ANSI_RED << "\"" << command << "\" is not a valid command.\n" << ANSI_NORMAL;
	}

}

void helpWith1Arg(const string& specific) {
	if (specific == "commands") {
		cout << ANSI_GREEN << COMMAND_HELP_DIRECTORY << "\n" << ANSI_NORMAL;
	} else if (specific == "variables") {
		cout << ANSI_GREEN << VARIABLE_HELP << "\n" << ANSI_NORMAL;
	} else if (specific == "lclinfo") {
		cout << ANSI_GREEN << LCL_INFO << "\n" << ANSI_NORMAL;
	} else {
		cout << ANSI_RED << "\"/help " << specific << "\" does not display a valid help document.\n" << ANSI_NORMAL;
	}
}

void helpWith2Arg(const string& spec1, const string& spec2) {
	if (spec1 == "commands") {
		if (spec2 == "general") {
			cout << ANSI_GREEN << COMMAND_HELP_GENERAL << "\n" << ANSI_NORMAL;
		} else if (spec2 == "variables") {
			cout << ANSI_GREEN << COMMAND_HELP_VARIABLES << "\n" << ANSI_NORMAL;
		} else if (spec2 == "arithmetic") {
			cout << ANSI_GREEN << COMMAND_HELP_ARITHMETIC << "\n" << ANSI_NORMAL;
		} else {
			cout << ANSI_RED << "\"/help commands " << spec2 << "\" does not display a valid help document, because \""
			 	 << spec2 << "\" is not a valid category of commands.\n" << ANSI_NORMAL;
		}
	} else {
		cout << ANSI_RED << "\"/help " << spec1 << " " << spec2 << "\" does not display a valid help document.\n" << ANSI_NORMAL;
	}
}

void line(int n) {
	for (int i = 1; i <= n; i++) {
		cout << "\n";
	}
}

void createVar(const string& name, const string& val, const string& type) {
	if (!contains(vars, name)) {
		// create new variable
		vars.push_back(Variable(name, val, type));
	} else {
		// overwrite old variable with new variable
		Variable& var = find(vars, name);
		var.value = val;
		if (var.datatype != type) {
			if (warn_type_change) {
				cout << ANSI_RED << "Alert! Variable overwrite changes its type.\n" << ANSI_NORMAL;
			}
			var.datatype = type;
		}
	}
}

void updateVar(const string& name, const string& val) {
	// requires that the variable already exists, and the new value is of the same type as the old value of the variable.
	// if the variable does not already exist, nothing too bad will happen and an error message will be displayed.
	// however, if the variable does exist but the new value's type is not the same as the old, then it will be UNDEFINED BEHAVIOUR!
	if (!contains(vars, name)) {
		cout << ANSI_RED << "You are trying to update a non-existent variable.\n"
			    "Use /store with 3 arguments (including type) if you want to create a new variable.\n" << ANSI_NORMAL;
		return;
	}
	// overwrite old value with new value
	Variable& var = find(vars, name);
	var.value = val;
}

void digits(int n) {
	cout << fixed << setprecision(n);
	num_places = n;
}

void printVar(const string& varname) {
	if (contains(vars, varname)) {
		cout << find(vars, varname) << "\n";
	} else {
		cout << ANSI_RED << "There is currently no variable named \"" << varname << "\"\n" << ANSI_NORMAL;
	}
}

void printVarValue(const string& varname) {
	if (contains(vars, varname)) {
		Variable var = find(vars, varname);
		if (var.datatype == "String" || var.datatype == "Bool") {
			cout << var.value;
		} else if (var.datatype == "Number") {
			cout << stod(var.value);
		}
	} else {
		cout << ANSI_RED << "There is currently no variable named \"" << varname << "\"\n" << ANSI_NORMAL;
	}
}

void digitCommand(int n) {
	if (0 <= n && n <= 12) {
		digits(n);
	} else {
		cout << ANSI_RED << "\"/digits " << n << "\" is not valid because /digits requires 0 <= n <= 12\n" << ANSI_NORMAL;
	}
}

void add(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	double result = parseNumericalArgument(command, 2);
	for (int i = 3; i <= numArgs; i++) {
		result += parseNumericalArgument(command, i);
	}
	createVar(varname, doubleToString(result), "Number");
}

void sub(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	double result = parseNumericalArgument(command, 2);
	for (int i = 3; i <= numArgs; i++) {
		result -= parseNumericalArgument(command, i);
	}
	createVar(varname, doubleToString(result), "Number");
}

void times(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	double result = parseNumericalArgument(command, 2);
	for (int i = 3; i <= numArgs; i++) {
		result *= parseNumericalArgument(command, i);
	}
	createVar(varname, doubleToString(result), "Number");
}

void div(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	double result = parseNumericalArgument(command, 2);
	for (int i = 3; i <= numArgs; i++) {
		result /= parseNumericalArgument(command, i);
	}
	createVar(varname, doubleToString(result), "Number");
}

void fdiv(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	int result = roundToNearestInt(parseNumericalArgument(command, 2));
	for (int i = 3; i <= numArgs; i++) {
		result /= roundToNearestInt(parseNumericalArgument(command, i));
	}
	createVar(varname, doubleToString(result), "Number");
}

void mod(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	int result = roundToNearestInt(parseNumericalArgument(command, 2));
	for (int i = 3; i <= numArgs; i++) {
		result %= roundToNearestInt(parseNumericalArgument(command, i));
	}
	createVar(varname, doubleToString(result), "Number");
}

void round(const string& command) {
	string varname = parseArgument(command, 1);
	int result = roundToNearestInt(parseNumericalArgument(command, 2));
	createVar(varname, doubleToString(result), "Number");
}

// THIS FUNCTION IS USED ONLY FOR TESTING, IT WILL BE EXECUTED WHEN THE MAIN PROGRAM TERMINATES
// note: this function passes in the commands of the program by reference, so we can look at the commands for debugging purposes
// note: some commands will be different when inspected at the end of execution because of modifications during the program execution
void test(vector<string>& com) {
	// nothing for now
}