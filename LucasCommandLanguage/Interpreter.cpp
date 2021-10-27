#include "file.h"
#include "parseArguments.h"
#include "vectorUtility.h"
#include "stringUtility.h"
#include "help.h"
#include "variable.h"
#include "ansi_codes.h"
#include "numberUtility.h"
#include "block.h"

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
using namespace blk;

const string INPUT_FILE = "../LucasCommandLanguage/inputFile/input.txt"; // file path for the source code

bool reuse_display = true; // using "/prev" will show special green message
bool warn_type_change = true; // changing type of variable will show warning message
bool use_blue = true; // most output is blue (instead of the system's default colour)

int num_places = 3; // number of decimal places numerical display uses

vector<Variable> vars; // list of all variables in the program

bool block_mode_on = false; // the current command is being placed into a block instead of being executed normally
string curr_block_name; // name of the current block being defined
vector<string> temp_commands; // all the commands in the current block being defined (so far)
vector<Block> blocks; // list of all blocks in the program

// primary function for interpreting commands
void interpretCommand(const string& command, vector<string>& commands, int currIndex);

// a testing function that executes when the main program terminates
void test(vector<string>& com);

// general helpers
void helpWith1Arg(const string& specific);
void helpWith2Arg(const string& spec1, const string& spec2);
void line(int n);
void digits(int n);
void digitCommand(int n);
void inputWithPrompt(const string& varname, const string& type, const string& prompt);

// variable-related helpers
void createVar(const string& name, const string& val, const string& type);
void updateVar(const string& name, const string& val);
void printVar(const string& varname);
void printVarValue(const string& varname);

// arithmetic helpers
void add(const string& command);
void sub(const string& command);
void times(const string& command);
void div(const string& command);
void fdiv(const string& command);
void mod(const string& command);
void round(const string& command);

// relational helpers
void lessThan(const string& command);
void greaterThan(const string& command);
void lessEqual(const string& command);
void greaterEqual(const string& command);
void numEqual(const string& command);
void numNotEqual(const string& command);

// logical helpers
void logicalAnd(const string& command);
void logicalOr(const string& command);
void logicalXor(const string& command);
void boolEqual(const string& command);
void logicalNot(const string& command);

// casting/conversion helpers
void numToBool(const string& command);
void boolToNum(const string& command);
void numToStr(const string& command);
void strToNum(const string& command);
void boolToStr(const string& command);
void strToBool(const string& command);

// string-related helpers
void strLength(const string& varname, const string& s);
void strContains(const string& varname, const string& s1, const string& s2);
void strCharAt(const string& varname, const string& s, int i);
void strConcat(const string& command);
void strSubstr(const string& varname, const string& s, int begin, int end);
void strEqual(const string& command);

// block-related helpers
void createBlock(const string& blockname);
void runBlock(const string& blockname, vector<string>& commands, int currIndex);

// control-flow-related helpers
void cflowIf(bool condition, const string& blockname, vector<string>& commands, int currIndex);
void cflowIfvar(const string& boolvarname, const string& blockname, vector<string>& commands, int currIndex);
void cflowLoop(int numIterations, const string& blockname, vector<string>& commands, int currIndex);
void cflowFor(const string& numvarname, const string& blockname, vector<string>& commands, int currIndex);
void cflowWhile(const string& condvarname, const string& blockname, vector<string>& commands, int currIndex);

// memory related helpers
void delVar(const string& varname);
void delBlock(const string& blockname);
void cleanTemps();
void existVar(const string& boolvarname, const string& varname);
void existBlock(const string& boolvarname, const string& blockname);
void existTemps(const string& boolvarname);

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

			if (block_mode_on) {
				if (command == "/endblock") {
					block_mode_on = false;
					Block thisBlock(curr_block_name, temp_commands);
					curr_block_name.clear();
					temp_commands.clear();
					blocks.push_back(thisBlock);
					continue;
				} else {
					temp_commands.push_back(command);
					continue;
				}
			}

			if (commandIs(command, "/escprint")) {
				string message = parseArgumentUntilEnd(command);
				if (use_blue) {
					cout << ANSI_BLUE;
				}
				cout << message << "\n" << ANSI_NORMAL;
				continue;
			}

			replaceVariableReferencesWithRoundedValues(command, vars, num_places);

			if (commandIs(command, "/escvarprint")) {
				string message = parseArgumentUntilEnd(command);
				if (use_blue) {
					cout << ANSI_BLUE;
				}
				cout << message << "\n" << ANSI_NORMAL;
				continue;
			}

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

			interpretCommand(command, commands, currIndex);

		}

	} catch (...) {
		cout << ANSI_RED << "Program has crashed unexpectedly.\n"
			 << "The command that crashed it is \"" << command << "\"\n" << ANSI_NORMAL;
	}

}

void interpretCommand(const string& command, vector<string>& commands, int currIndex) {

	if (beginsWith(command, "//")) { // cannot use commandIs() because there may not be a space between the "//" and the comment itself
		// ignore this command because it is a comment
	} else if (commandIs(command, "/help") && numArguments(command) == 0) {
		cout << ANSI_GREEN << HELP_DIRECTORY << "\n" << ANSI_NORMAL;
	} else if (commandIs(command, "/help") && numArguments(command) == 1) {
		helpWith1Arg(parseArgument(command));
	} else if (commandIs(command, "/help") && numArguments(command) == 2) {
		helpWith2Arg(parseArgument(command, 1), parseArgument(command, 2));
	} else if (commandIs(command, "/line") && numArguments(command) == 0) {
		line(1);
	} else if (commandIs(command, "/line") && numArguments(command) == 1) {
		line(parseNumericalArgument(command));
	} else if (commandIs(command, "/digits")) {
		digitCommand(parseNumericalArgument(command));
	} else if (commandIs(command, "/warntype")) {
		warn_type_change = parseBooleanArgument(command);
	} else if (commandIs(command, "/useblue")) {
		use_blue = parseBooleanArgument(command);
	} else if (commandIs(command, "/reusedisp")) {
		reuse_display = parseBooleanArgument(command);
	} else if (commandIs(command, "/input")) {
		inputWithPrompt(parseArgument(command, 1), parseArgument(command, 2), parseArgumentUntilEnd(command, 3));
	} else if (commandIs(command, "/store") && numArguments(command) == 3) {
		createVar(parseArgument(command, 1), parseArgument(command, 2), parseArgument(command, 3));
	} else if (commandIs(command, "/store") && numArguments(command) == 2) {
		updateVar(parseArgument(command, 1), parseArgument(command, 2));
	} else if (commandIs(command, "/printvar")) {
		printVar(parseArgument(command));
	} else if (commandIs(command, "/varval")) {
		printVarValue(parseArgument(command));
	} else if (commandIs(command, "/add")) {
		add(command);
	} else if (commandIs(command, "/sub")) {
		sub(command);
	} else if (commandIs(command, "/times")) {
		times(command);
	} else if (commandIs(command, "/div")) {
		div(command);
	} else if (commandIs(command, "/fdiv")) {
		fdiv(command);
	} else if (commandIs(command, "/mod")) {
		mod(command);
	} else if (commandIs(command, "/round")) {
		round(command);
	} else if (commandIs(command, "/less")) {
		lessThan(command);
	} else if (commandIs(command, "/greater")) {
		greaterThan(command);
	} else if (commandIs(command, "/le")) {
		lessEqual(command);
	} else if (commandIs(command, "/ge")) {
		greaterEqual(command);
	} else if (commandIs(command, "/equal")) {
		numEqual(command);
	} else if (commandIs(command, "/notequal")) {
		numNotEqual(command);
	} else if (commandIs(command, "/and")) {
		logicalAnd(command);
	} else if (commandIs(command, "/or")) {
		logicalOr(command);
	} else if (commandIs(command, "/xor")) {
		logicalXor(command);
	} else if (commandIs(command, "/boolequal")) {
		boolEqual(command);
	} else if (commandIs(command, "/not")) {
		logicalNot(command);
	} else if (commandIs(command, "/numtobool")) {
		numToBool(command);
	} else if (commandIs(command, "/booltonum")) {
		boolToNum(command);
	} else if (commandIs(command, "/numtostr")) {
		numToStr(command);
	} else if (commandIs(command, "/strtonum")) {
		strToNum(command);
	} else if (commandIs(command, "/booltostr")) {
		boolToStr(command);
	} else if (commandIs(command, "/strtobool")) {
		strToBool(command);
	} else if (commandIs(command, "/length")) {
		strLength(parseArgument(command, 1), parseArgument(command, 2));
	} else if (commandIs(command, "/contains")) {
		strContains(parseArgument(command, 1), parseArgument(command, 2), parseArgument(command, 3));
	} else if (commandIs(command, "/charat")) {
		strCharAt(parseArgument(command, 1), parseArgument(command, 2), parseNumericalArgument(command, 3));
	} else if (commandIs(command, "/concat")) {
		strConcat(command);
	} else if (commandIs(command, "/substr")) {
		strSubstr(parseArgument(command, 1), parseArgument(command, 2), parseNumericalArgument(command, 3), parseNumericalArgument(command, 4));
	} else if (commandIs(command, "/strequal")) {
		strEqual(command);
	} else if (commandIs(command, "/blockdef")) {
		createBlock(parseArgument(command));
	} else if (commandIs(command, "/block")) {
		runBlock(parseArgument(command), commands, currIndex);
	} else if (commandIs(command, "/if")) {
		cflowIf(parseBooleanArgument(command, 1), parseArgument(command, 2), commands, currIndex);
	} else if (commandIs(command, "/ifvar")) {
		cflowIfvar(parseArgument(command, 1), parseArgument(command, 2), commands, currIndex);
	} else if (commandIs(command, "/loop")) {
		cflowLoop(parseNumericalArgument(command, 1), parseArgument(command, 2), commands, currIndex);
	} else if (commandIs(command, "/for")) {
		cflowFor(parseArgument(command, 1), parseArgument(command, 2), commands, currIndex);
	} else if (commandIs(command, "/while")) {
		cflowWhile(parseArgument(command, 1), parseArgument(command, 2), commands, currIndex);
	} else if (commandIs(command, "/delvar")) {
		delVar(parseArgument(command));
	} else if (commandIs(command, "/delblock")) {
		delBlock(parseArgument(command));
	} else if (commandIs(command, "/clean")) {
		cleanTemps();
	} else if (commandIs(command, "/existvar")) {
		existVar(parseArgument(command, 1), parseArgument(command, 2));
	} else if (commandIs(command, "/existblock")) {
		existBlock(parseArgument(command, 1), parseArgument(command, 2));
	} else if (commandIs(command, "/existtemps")) {
		existTemps(parseArgument(command));
	} else {
		cout << ANSI_RED << "\"" << command << "\" is not a valid command.\n" << ANSI_NORMAL;
	}

}

// THIS FUNCTION IS USED ONLY FOR TESTING, IT WILL BE EXECUTED WHEN THE MAIN PROGRAM TERMINATES
// note: this function passes in the commands of the program by reference, so we can look at the commands for debugging purposes
// note: some commands will be different when inspected at the end of execution because of modifications during the program execution
void test(vector<string>& com) {
	/*
	cout << ANSI_YELLOW << "\nVariables:\n\n" << ANSI_NORMAL;
	for (const Variable& var : vars) {
		cout << var.name << "\n";
	}
	if (vars.empty()) {
		cout << ANSI_MAGENTA << "(none)\n" << ANSI_NORMAL;
	}
	cout << ANSI_YELLOW << "\nBlocks:\n\n" << ANSI_NORMAL;
	for (const Block& block : blocks) {
		cout << block.name << "\n";
	}
	if (blocks.empty()) {
		cout << ANSI_MAGENTA << "(none)\n" << ANSI_NORMAL;
	}
	*/
}

void helpWith1Arg(const string& specific) {
	if (specific == "commands") {
		cout << ANSI_GREEN << COMMAND_HELP_DIRECTORY << "\n" << ANSI_NORMAL;
	} else if (specific == "variables") {
		cout << ANSI_GREEN << VARIABLE_HELP << "\n" << ANSI_NORMAL;
	} else if (specific == "blocks") {
		cout << ANSI_GREEN << BLOCK_HELP << "\n" << ANSI_NORMAL;
	} else if (specific == "memory") {
		cout << ANSI_GREEN << MEM_HELP << "\n" << ANSI_NORMAL;
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
		} else if (spec2 == "relational") {
			cout << ANSI_GREEN << COMMAND_HELP_RELATIONAL << "\n" << ANSI_NORMAL;
		} else if (spec2 == "logical") {
			cout << ANSI_GREEN << COMMAND_HELP_LOGICAL << "\n" << ANSI_NORMAL;
		} else if (spec2 == "cast") {
			cout << ANSI_GREEN << COMMAND_HELP_CAST << "\n" << ANSI_NORMAL;
		} else if (spec2 == "string") {
			cout << ANSI_GREEN << COMMAND_HELP_STRING << "\n" << ANSI_NORMAL;
		} else if (spec2 == "blocks") {
			cout << ANSI_GREEN << COMMAND_HELP_BLOCKS << "\n" << ANSI_NORMAL;
		} else if (spec2 == "cflow") {
			cout << ANSI_GREEN << COMMAND_HELP_CONTROL_FLOW << "\n" << ANSI_NORMAL;
		} else if (spec2 == "memory") {
			cout << ANSI_GREEN << COMMAND_HELP_MEMORY << "\n" << ANSI_NORMAL;
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

void digits(int n) {
	cout << fixed << setprecision(n);
	num_places = n;
}

void digitCommand(int n) {
	if (0 <= n && n <= 12) {
		digits(n);
	} else {
		cout << ANSI_RED << "\"/digits " << n << "\" is not valid because /digits requires 0 <= n <= 12\n" << ANSI_NORMAL;
	}
}

void inputWithPrompt(const string& varname, const string& type, const string& prompt) {
	if (use_blue) {
		cout << ANSI_BLUE;
	}
	cout << prompt << ANSI_NORMAL;
	string rawinput;
	cin >> rawinput;
	if (type == "String") {
		createVar(varname, rawinput, type);
	} else if (type == "Number") {
		createVar(varname, doubleToString(stod(rawinput)), type);
	} else if (type == "Bool") {
		createVar(varname, boolval(parseBool(rawinput)), type);
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

void lessThan(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	vector<double> v;
	for (int i = 2; i <= numArgs; i++) {
		v.push_back(parseNumericalArgument(command, i));
	}
	bool less = strictlyIncreasing(v);
	createVar(varname, boolval(less), "Bool");
}

void greaterThan(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	vector<double> v;
	for (int i = 2; i <= numArgs; i++) {
		v.push_back(parseNumericalArgument(command, i));
	}
	bool greater = strictlyDecreasing(v);
	createVar(varname, boolval(greater), "Bool");
}

void lessEqual(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	vector<double> v;
	for (int i = 2; i <= numArgs; i++) {
		v.push_back(parseNumericalArgument(command, i));
	}
	bool lessEq = generallyIncreasing(v);
	createVar(varname, boolval(lessEq), "Bool");
}

void greaterEqual(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	vector<double> v;
	for (int i = 2; i <= numArgs; i++) {
		v.push_back(parseNumericalArgument(command, i));
	}
	bool greaterEq = generallyDecreasing(v);
	createVar(varname, boolval(greaterEq), "Bool");
}

void numEqual(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	vector<double> v;
	for (int i = 2; i <= numArgs; i++) {
		v.push_back(parseNumericalArgument(command, i));
	}
	bool eq = allEqual(v);
	createVar(varname, boolval(eq), "Bool");
}

void numNotEqual(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	vector<double> v;
	for (int i = 2; i <= numArgs; i++) {
		v.push_back(parseNumericalArgument(command, i));
	}
	bool notEq = atLeast1NotEqual(v);
	createVar(varname, boolval(notEq), "Bool");
}

void logicalAnd(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	bool result = true;
	for (int i = 2; i <= numArgs; i++) {
		result = result && parseBooleanArgument(command, i);
	}
	createVar(varname, boolval(result), "Bool");
}

void logicalOr(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	bool result = false;
	for (int i = 2; i <= numArgs; i++) {
		result = result || parseBooleanArgument(command, i);
	}
	createVar(varname, boolval(result), "Bool");
}

void logicalXor(const string& command) {
	string varname = parseArgument(command, 1);
	bool left = parseBooleanArgument(command, 2);
	bool right = parseBooleanArgument(command, 3);
	bool result = (left != right);
	createVar(varname, boolval(result), "Bool");
}

void boolEqual(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	vector<bool> v;
	for (int i = 2; i <= numArgs; i++) {
		v.push_back(parseBooleanArgument(command, i));
	}
	bool eq = allEqual(v);
	createVar(varname, boolval(eq), "Bool");
}

void logicalNot(const string& command) {
	string varname = parseArgument(command, 1);
	bool result = !parseBooleanArgument(command, 2);
	createVar(varname, boolval(result), "Bool");
}

void numToBool(const string& command) {
	string varname = parseArgument(command, 1);
	double num = parseNumericalArgument(command, 2);
	bool result = (num == 0) ? false : true;
	createVar(varname, boolval(result), "Bool");
}

void boolToNum(const string& command) {
	string varname = parseArgument(command, 1);
	bool b = parseBooleanArgument(command, 2);
	double result = b ? 1 : 0;
	createVar(varname, doubleToString(result), "Number");
}

void numToStr(const string& command) {
	string varname = parseArgument(command, 1);
	double num = parseNumericalArgument(command, 2);
	string result = toStringRemoveTrailingZeros(num);
	createVar(varname, result, "String");
}

void strToNum(const string& command) {
	string varname = parseArgument(command, 1);
	string str = parseArgument(command, 2);
	double result = stod(str);
	createVar(varname, doubleToString(result), "Number");
}

void boolToStr(const string& command) {
	string varname = parseArgument(command, 1);
	bool b = parseBooleanArgument(command, 2);
	string result = boolval(b);
	createVar(varname, result, "String");
}

void strToBool(const string& command) {
	string varname = parseArgument(command, 1);
	string str = parseArgument(command, 2);
	bool result = parseBool(str);
	createVar(varname, boolval(result), "Bool");
}

void strLength(const string& varname, const string& s) {
	createVar(varname, doubleToString(s.size()), "Number");
}

void strContains(const string& varname, const string& s1, const string& s2) {
	createVar(varname, boolval(contains(s1, s2)), "Bool");
}

void strCharAt(const string& varname, const string& s, int i) {
	string result = string() + s.at(i);
	createVar(varname, result, "String");
}

void strConcat(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	string result = parseArgument(command, 2);
	for (int i = 3; i <= numArgs; i++) {
		result += parseArgument(command, i);
	}
	createVar(varname, result, "String");
}

void strSubstr(const string& varname, const string& s, int begin, int end) {
	string result = s.substr(begin, end - begin + 1);
	createVar(varname, result, "String");
}

void strEqual(const string& command) {
	string varname = parseArgument(command, 1);
	int numArgs = numArguments(command);
	vector<string> v;
	for (int i = 2; i <= numArgs; i++) {
		v.push_back(parseArgument(command, i));
	}
	bool eq = allEqual(v);
	createVar(varname, boolval(eq), "Bool");
}

void createBlock(const string& blockname) {
	block_mode_on = true;
	curr_block_name = blockname;
}

void runBlock(const string& blockname, vector<string>& commands, int currIndex) {
	Block block = find(blocks, blockname);
	block.spliceInto(commands, currIndex);
}

void cflowIf(bool condition, const string& blockname, vector<string>& commands, int currIndex) {
	if (condition) {
		runBlock(blockname, commands, currIndex);
	}
}

void cflowIfvar(const string& boolvarname, const string& blockname, vector<string>& commands, int currIndex) {
	if (!contains(vars, boolvarname)) {
		cout << ANSI_RED << "/ifvar or /while cannot execute because the boolean variable \"" << boolvarname << "\" is not found.\n" << ANSI_NORMAL;
		return;
	}
	Variable boolvar = find(vars, boolvarname);
	if (boolvar.datatype != "Bool") {
		cout << ANSI_RED << "/ifvar or /while cannot execute because the variable \"" << boolvarname << "\" is not of type Bool.\n" << ANSI_NORMAL;
		return;
	}
	cflowIf(boolvar.getBooleanValue(), blockname, commands, currIndex);
}

void cflowLoop(int numIterations, const string& blockname, vector<string>& commands, int currIndex) {
	for (int i = 1; i <= numIterations; i++) {
		runBlock(blockname, commands, currIndex);
	}
}

void cflowFor(const string& numvarname, const string& blockname, vector<string>& commands, int currIndex) {
	if (!contains(vars, numvarname)) {
		cout << ANSI_RED << "/for cannot execute because the numerical variable \"" << numvarname << "\" is not found.\n" << ANSI_NORMAL;
		return;
	}
	Variable numvar = find(vars, numvarname);
	if (numvar.datatype != "Number") {
		cout << ANSI_RED << "/for cannot execute because the variable \"" << numvarname << "\" is not of type Number.\n" << ANSI_NORMAL;
		return;
	}
	cflowLoop(numvar.getNumericalValue(), blockname, commands, currIndex);
}

/*

Given any block defined as follows (where <loc> indicates an arbitrary number of commands):

/blockdef B
<loc>
/endblock

The command:
/while <conditionvariable> B

can be rewritten as:
/ifvar <conditionvariable> B'

where B' is a recursively defined block:

/blockdef B'
/block B
/ifvar <conditionvariable> B'
/endblock

Such a transformation allows us to perform while loops.

*/
void cflowWhile(const string& condvarname, const string& blockname, vector<string>& commands, int currIndex) {
	
	// make new block that starts with the reserved prefix ("__temp__")
	Block block = find(blocks, blockname);
	string newBlockname = block.createTemp(blocks, condvarname);

	// run the transformed version of the block
	cflowIfvar(condvarname, newBlockname, commands, currIndex);

}

void delVar(const string& varname) {
	var::remove(vars, varname);
}

void delBlock(const string& blockname) {
	blk::remove(blocks, blockname);
}

void cleanTemps() {
	vector<string> namesOfBlocksToDelete;
	for (const Block& block : blocks) {
		if (beginsWith(block.name, blk::TEMP_BLOCK_PREFIX)) {
			namesOfBlocksToDelete.push_back(block.name);
		}
	}
	for (const string& nameToDelete : namesOfBlocksToDelete) {
		blk::remove(blocks, nameToDelete);
	}
}

void existVar(const string& boolvarname, const string& varname) {
	bool exist = var::contains(vars, varname);
	createVar(boolvarname, boolval(exist), "Bool");
}

void existBlock(const string& boolvarname, const string& blockname) {
	bool exist = blk::contains(blocks, blockname);
	createVar(boolvarname, boolval(exist), "Bool");
}

void existTemps(const string& boolvarname) {
	bool exist = false;
	for (const Block& block : blocks) {
		if (beginsWith(block.name, blk::TEMP_BLOCK_PREFIX)) {
			exist = true;
			break;
		}
	}
	createVar(boolvarname, boolval(exist), "Bool");
}