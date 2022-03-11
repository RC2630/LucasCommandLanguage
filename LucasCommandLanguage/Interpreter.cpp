#include "file.h"
#include "parseArguments.h"
#include "vectorUtility.h"
#include "stringUtility.h"
#include "ansi_codes.h"
#include "numberUtility.h"
#include "abstractFunctions.h"

#include "help.h"
#include "variable.h"
#include "block.h"
#include "struct.h"

#include <iomanip>
#include <stdexcept>

using namespace std;

using namespace file;
using namespace parse;
using namespace vecUtil;
using namespace strUtil;
using namespace numUtil;
using namespace absFunc;

using namespace help;
using namespace var;
using namespace blk;
using namespace srt;

const string INPUT_FILE = "../LucasCommandLanguage/inputFile/input.txt"; // file path for the source code

bool reuse_display = true; // using "/prev" will show special green message
bool warn_type_change = true; // changing type of variable will show warning message
bool use_blue = true; // most output is blue (instead of the system's default colour)
bool non_assert_crash = true; // program crashed NOT because of an assertion
bool debug_on_termination = false; // program will output debug information when it terminates

int num_places = 3; // number of decimal places numerical display uses

string top_level_crash_message = "none"; // the top-level crash message that results from the exception that triggered main()'s catch-all

vector<Variable> vars; // list of all variables in the program

bool block_mode_on = false; // the current command is being placed into a block instead of being executed normally
string curr_block_name; // name of the current block being defined
vector<string> temp_commands; // all the commands in the current block being defined (so far)
vector<Block> blocks; // list of all blocks in the program

vector<Struct> structs; // list of all structs in the program
vector<Object> objects; // list of all objects in the program

// primary function for interpreting commands
void interpretCommand(const string& command, vector<string>& commands, int currIndex, const string& untouched);

// a testing function that executes when the main program terminates (or crashes)
void debugOnTermination(vector<string>& com);

// general helpers
void helpWith1Arg(const string& specific);
void helpWith2Arg(const string& spec1, const string& spec2);
void line(int n);
void space(int n);
void tab(int n);
void digits(int n);
void digitCommand(int n);
void inputWithPrompt(const string& varname, const string& type, const string& prompt);

// command preprocessing functions
bool isEscapeCommand(const string& command);
void removeIndents(string& command);
void removeInlineComments(string& command);
void spliceSemicolonCommands(string& command, vector<string>& commands, int spliceAtIndex);
void lineContinuation(string& command, vector<string>& commands, int currIndex);
bool hasComment(const string& command);
bool endsWithDots(const string& command);
void preprocess(string& command, vector<string>& commands, int currIndex);

// variable-related helpers
void createVar(const string& name, const string& val, const string& type);
void updateVar(const string& name, const string& val);
void printVar(const string& varname);
void printVarValue(const string& varname);
void getType(const string& strvar, const string& varname);

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

// memory related helpers (part 1: deletion helpers)
void delVar(const string& varname);
void delBlock(const string& blockname);
void delStruct(const string& srtname);
void delObject(const string& objname);
void cleanTemps();

// memory related helpers (part 2: existence helpers)
void existVar(const string& boolvarname, const string& varname);
void existBlock(const string& boolvarname, const string& blockname);
void existStruct(const string& boolvarname, const string& srtname);
void existObject(const string& boolvarname, const string& objname);
void existTemps(const string& boolvarname);

// assertion helpers
void customFail(const string& command, const string& message);
void customAssert(const string& command, bool boolvalue, const string& message);
void customAssertVar(const string& command, const string& boolvarname, const string& message);
void customAssertType(const string& command, const string& varname, const string& type, const string& message);
void customAssertObjectType(const string& command, const string& objname, const string& srtname, const string& message);

// struct and object helpers
void structdef(const string& command);
void construct(const string& command);
void setDefault(const string& command);
void consDefault(const string& command);
void copyAssignment(const Object& sourceObject, const Object& destObject);
void nestedCopyConstruction(const Object& sourceNestedObject, vector<string>& fieldInitValues);
void copyConstruction(const Object& sourceObject, const string& destObjname);
void copyObject(const string& command);
void getObjectType(const string& strvar, const string& objname);
void inherit(const string& command);
void stringRep(const string& srtname, const string& rep);

int main() {

	digits(num_places);
	
	vector<string> commands;
	inputStrVecFrom(commands, INPUT_FILE);
	commands.push_back("/stop");
	
	string command;
	string untouchedCommand;
	string prevCommand;
	int currIndex = 0;

	cout << "\n";

	if (commands.size() == 1) { // the only command is "/stop" (i.e. the source code is empty)
		cout << ANSI_GREEN << "The source code is empty.\n"
			 << "Type " << ANSI_YELLOW << "/help" << ANSI_GREEN << " in the source code if you need help getting started with this programmming language.\n\n" << ANSI_NORMAL;
	}

	try {

		while (true) {

			prevCommand = command;
			command = commands.at(currIndex);
			currIndex++;
			untouchedCommand = command;

			preprocess(command, commands, currIndex - 1); // the true current index is currIndex - 1, because currIndex is the index of the next instruction at this point
			
			if (beginsWith(command, "//")) {
				// it's just a comment
				continue;
			}

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

			replaceVariableReferencesWithRoundedValues(command, vars, objects, structs, num_places);

			if (commandIs(command, "/escvarprint")) {
				string message = parseArgumentUntilEnd(command);
				if (use_blue) {
					cout << ANSI_BLUE;
				}
				cout << message << "\n" << ANSI_NORMAL;
				continue;
			}

			replaceVariableReferencesWithFullPrecisionValues(command, vars, objects, structs);

			if (command == "/prev") {
				if (prevCommand == "" || prevCommand == "/prev") {
					cout << ANSI_RED << "Sorry, but there is no previous command.\n" << ANSI_NORMAL;
					continue;
				} else {
					if (reuse_display) {
						cout << ANSI_GREEN << "Reusing previous command " << ANSI_YELLOW << prevCommand << ANSI_GREEN << " ......\n" << ANSI_NORMAL;
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
				if (debug_on_termination) {
					debugOnTermination(commands);
				}
				break;
			}

			interpretCommand(command, commands, currIndex, untouchedCommand);

		}

	} catch (const exception& e) {
		if (non_assert_crash) {
			cout << ANSI_RED << "Program has crashed unexpectedly.\n"
				 << "The command that crashed it is " << ANSI_YELLOW << untouchedCommand << ANSI_RED << ".\n" << ANSI_NORMAL;
		}
		top_level_crash_message = e.what();
		if (debug_on_termination) {
			debugOnTermination(commands);
		}
	}

}

void interpretCommand(const string& command, vector<string>& commands, int currIndex, const string& untouched) {

	if (commandIs(command, "/help") && numArguments(command) == 0) {
		cout << ANSI_GREEN << HELP_DIRECTORY << "\n" << ANSI_NORMAL;
	} else if (commandIs(command, "/help") && numArguments(command) == 1) {
		helpWith1Arg(parseArgument(command));
	} else if (commandIs(command, "/help") && numArguments(command) == 2) {
		helpWith2Arg(parseArgument(command, 1), parseArgument(command, 2));
	} else if (commandIs(command, "/line") && numArguments(command) == 0) {
		line(1);
	} else if (commandIs(command, "/line") && numArguments(command) == 1) {
		line(parseNumericalArgument(command));
	} else if (commandIs(command, "/space") && numArguments(command) == 0) {
		space(1);
	} else if (commandIs(command, "/space") && numArguments(command) == 1) {
		space(parseNumericalArgument(command));
	} else if (commandIs(command, "/tab") && numArguments(command) == 0) {
		tab(1);
	} else if (commandIs(command, "/tab") && numArguments(command) == 1) {
		tab(parseNumericalArgument(command));
	} else if (commandIs(command, "/digits")) {
		digitCommand(parseNumericalArgument(command));
	} else if (commandIs(command, "/warntype")) {
		warn_type_change = parseBooleanArgument(command);
	} else if (commandIs(command, "/useblue")) {
		use_blue = parseBooleanArgument(command);
	} else if (commandIs(command, "/reusedisp")) {
		reuse_display = parseBooleanArgument(command);
	} else if (commandIs(command, "/debug")) {
		debug_on_termination = parseBooleanArgument(command);
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
	} else if (commandIs(command, "/gettype")) {
		getType(parseArgument(command, 1), parseArgument(command, 2));
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
	} else if (commandIs(command, "/delstruct")) {
		delStruct(parseArgument(command));
	} else if (commandIs(command, "/delobject")) {
		delObject(parseArgument(command));
	} else if (commandIs(command, "/clean")) {
		cleanTemps();
	} else if (commandIs(command, "/existvar")) {
		existVar(parseArgument(command, 1), parseArgument(command, 2));
	} else if (commandIs(command, "/existblock")) {
		existBlock(parseArgument(command, 1), parseArgument(command, 2));
	} else if (commandIs(command, "/existstruct")) {
		existStruct(parseArgument(command, 1), parseArgument(command, 2));
	} else if (commandIs(command, "/existobject")) {
		existObject(parseArgument(command, 1), parseArgument(command, 2));
	} else if (commandIs(command, "/existtemps")) {
		existTemps(parseArgument(command));
	} else if (commandIs(command, "/fail")) {
		customFail(commandPlusNargs(untouched, 0), parseArgumentUntilEnd(command));
	} else if (commandIs(command, "/assert")) {
		customAssert(commandPlusNargs(untouched, 1), parseBooleanArgument(command, 1), parseArgumentUntilEnd(command, 2));
	} else if (commandIs(command, "/assertvar")) {
		customAssertVar(commandPlusNargs(untouched, 1), parseArgument(command, 1), parseArgumentUntilEnd(command, 2));
	} else if (commandIs(command, "/asserttype")) {
		customAssertType(commandPlusNargs(untouched, 2), parseArgument(command, 1), parseArgument(command, 2), parseArgumentUntilEnd(command, 3));
	} else if (commandIs(command, "/assertobjecttype")) {
		customAssertObjectType(commandPlusNargs(untouched, 2), parseArgument(command, 1), parseArgument(command, 2), parseArgumentUntilEnd(command, 3));
	} else if (commandIs(command, "/structdef")) {
		structdef(command);
	} else if (commandIs(command, "/construct")) {
		construct(command);
	} else if (commandIs(command, "/setdefault")) {
		setDefault(command);
	} else if (commandIs(command, "/consdefault")) {
		consDefault(command);
	} else if (commandIs(command, "/copyobject")) {
		copyObject(command);
	} else if (commandIs(command, "/getobjecttype")) {
		getObjectType(parseArgument(command, 1), parseArgument(command, 2));
	} else if (commandIs(command, "/inherit")) {
		inherit(command);
	} else if (commandIs(command, "/stringrep")) {
		stringRep(parseArgument(command, 1), parseArgumentUntilEnd(command, 2));
	} else if (numArguments(command) == 0 && blk::contains(blocks, command.substr(1))) { // DO NOT MOVE - SHOULD HAVE LOWEST PRECEDENCE
		runBlock(command.substr(1), commands, currIndex);
	} else {
		cout << ANSI_YELLOW << untouched << ANSI_RED << " is not a valid command.\n" << ANSI_NORMAL;
	}

}

// THIS FUNCTION IS USED ONLY FOR TESTING, IT WILL BE EXECUTED WHEN THE MAIN PROGRAM TERMINATES (OR CRASHES)
// note: this function passes in the commands of the program by reference, so we can look at the commands for debugging purposes
// note: some commands will be different when inspected at the end of execution because of modifications during the program execution
void debugOnTermination(vector<string>& com) {
	
	cout << ANSI_YELLOW << "\nVariables:\n\n" << ANSI_NORMAL;
	for (const Variable& var : vars) {
		cout << var.name << ANSI_RED << " = " << ANSI_GREEN
			 << var.getAppropriateValue() << ANSI_RED << " (" << ANSI_BLUE
			 << var.datatype << ANSI_RED << ")\n" << ANSI_NORMAL;
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

	cout << ANSI_YELLOW << "\nStructs:\n\n" << ANSI_NORMAL;
	for (const Struct& srt : structs) {
		cout << srt.name << "\n";
	}
	if (structs.empty()) {
		cout << ANSI_MAGENTA << "(none)\n" << ANSI_NORMAL;
	}

	cout << ANSI_YELLOW << "\nObjects:\n\n" << ANSI_NORMAL;
	for (const Object& obj : objects) {
		cout << obj.name << "\n";
	}
	if (objects.empty()) {
		cout << ANSI_MAGENTA << "(none)\n" << ANSI_NORMAL;
	}

	if (top_level_crash_message == "none") {
		cout << ANSI_GREEN << "\nProgram execution finished without an exception.\n" << ANSI_NORMAL;
	} else {
		cout << ANSI_RED << "\nTop-level crash message: " << ANSI_YELLOW << top_level_crash_message << "\n" << ANSI_NORMAL;
	}
	
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
	} else if (specific == "special") {
		cout << ANSI_GREEN << SPECIAL_HELP << "\n" << ANSI_NORMAL;
	} else if (specific == "struct") {
		cout << ANSI_GREEN << STRUCT_HELP << "\n" << ANSI_NORMAL;
	} else {
		cout << ANSI_YELLOW << "/help " << specific << ANSI_RED << " does not display a valid help document.\n" << ANSI_NORMAL;
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
		} else if (spec2 == "assert") {
			cout << ANSI_GREEN << COMMAND_HELP_ASSERT << "\n" << ANSI_NORMAL;
		} else if (spec2 == "struct") {
			cout << ANSI_GREEN << COMMAND_HELP_STRUCT << "\n" << ANSI_NORMAL;
		} else {
			cout << ANSI_YELLOW << "/help commands " << spec2 << ANSI_RED << " does not display a valid help document, because \""
			 	 << spec2 << "\" is not a valid category of commands.\n" << ANSI_NORMAL;
		}
	} else {
		cout << ANSI_YELLOW << "/help " << spec1 << " " << spec2 << ANSI_RED << " does not display a valid help document.\n" << ANSI_NORMAL;
	}
}

void line(int n) {
	for (int i = 1; i <= n; i++) {
		cout << "\n";
	}
}

void space(int n) {
	cout << spaces(n);
}

void tab(int n) {
	for (int i = 1; i <= n; i++) {
		cout << "\t";
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
		cout << ANSI_YELLOW << "/digits " << n << ANSI_RED << " is not valid because " << ANSI_YELLOW << "/digits" << ANSI_RED << " requires 0 <= n <= 12.\n" << ANSI_NORMAL;
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

/*

 A command is considered an escape command if it is "/escprint" or "/escvarprint".
 Normally, these would appear at the beginning of a line (barring indentation).
 Since a pre-condition of this function is that indentation has already been removed, we will not consider it here.
 Another pre-condition is that the previous line in the program, if it ends with "...", should have its "..." be already processed.

 HOW THIS FUNCTION WORKS:

 This function is meant to return true if ANY command in the given input is an escape command.
 Taking the pre-conditions into account, we notice that the only way for a command to be an escape command,
 is for it to be (1) at the beginning of a line, or (2) following a semicolon.
 The (1) case is trivial. The (2) case needs more thought, though.

 In the (2) case, the semicolon could be escaped. Alternatively, the command right after the semicolon could also be escaped.
 If either of these is escaped, then the escape command in question is NOT actually an escape command.
 We will proceed by proving that the command right after the semicolon could only be escaped IFF the semicolon is escaped.

 To escape something, we need to either use a comment or an escape command.
 Both will have their escaping effect extend until the end of the line.
 We will assume that the semicolon is escaped, and the command right afterwards is not.
 The semicolon being escaped implies that there is a comment or escape command to the left of the semicolon in the line.
 We know that the command is right after the semicolon, and so it will be to the right of the semicolon.
 This is also to the right of the escaper.
 Since we don't have a way to unescape things at the moment, the escape effect will extend to the command in question as well.
 This causes a contradiction, so it's proved.

 Now, we will assume that the semicolon is not escaped, and the command is.
 The semicolon being not escaped implies that there is no escaper to the left of it.
 In order for the command to be escaped, it needs an escaper to the left of it.
 But the only things left of the command is (i) the semicolon and (ii) the things left of the semicolon.
 We know (ii) does not contain any escapers, from above. (i) is not an escaper because it's just a semicolon.
 And so the command cannot be escaped. Again, a contradiction, and another proof is now done.

 Combining the above 2 results, we get that the semicolon being escaped <-> the command following it being escaped.
 Therefore, we can simplify the cases of this function in the (2) case to the semicolon being either escaped, or not escaped.
 And we don't need to worry about the escaped-ness of the command following the ";" anymore.

 At this point, we can see the general structure of our function, in pseudocode:

	if (escape command at beginning of line) {
		return true
	} else {
		for all ";" in the line {
			if (the ";" is not escaped AND escape command immediately following the ";") {
				return true
			}
		}
		return false
	}

 Obviously, it is a bit more complicated than that.
 In order to know whether the ";" is escaped or not, we need to see if there is an escaper to the left of it.
 And to check if there is an escaper to the left of it, we need to use this function, which causes recursion and added complexity.
 And so can we avoid it?

 It turns out that we can instead try traversing the given input from left to right.
 By the time we reach a semicolon, the ";" is either escaped or not.
 If it is escaped, it is either escaped by a comment or an escape command.
 If it is escaped by an escape command, then the function should have returned true already,
 because the escape command should be to the left of the semicolon.
 If it is a comment, then we need to do something a little trickier, as we will soon see.

 If the semicolon is not escaped, then there is no escape command or comment to the left of it.
 We already showed that there cannot be an escape command to the left of it at this stage in the function.
 So if it's not escaped, there's no comment to the left of it, period. And vice versa. And so it becomes a biconditional again.
 In other words, there is a comment to the left of ";" <-> the ";" is escaped.

 If we reached a comment from walking left to right through the input, then we know the rest of the input will all be escaped.
 This is true regardless of whether the comment is escaped itself or not.
 We can show this formally.

 If the comment itself is not escaped, then the rest of the input will be escaped by the current comment in question.
 If the comment itself is escaped by a previous comment, then everything from the previous comment to the rest of the input will be escaped.
 If the comment itself is escaped by a previous escape command, then we should have returned true already, so this should not happen.

 And so we can safely assume the rest of the input is escaped AS SOON AS we hit a comment, regardless of the escaped-ness of that comment.

 Let's update our pseudocode:

	if (escape command at beginning of line) {
		return true
	} else {
		traverse the input from left to right {
			if (we encounter "//") {
				return false
			}
			if (we encounter ";" AND escape command immediately following the ";") {
				return true
			}
		}
		return false
	}

 Now, this is much more implementable, and so let's get started with the implementation, then!

*/
bool isEscapeCommand(const string& command) {
	if (commandIs(command, "/escprint") || commandIs(command, "/escvarprint")) {
		return true;
	} else {
		bool slash = false;
		for (int i = 0; i < command.size(); i++) {

			if (command.at(i) == '/') {
				if (!slash) { // first slash
					slash = true;
				} else { // second slash
					return false;
				}
			} else {
				slash = false; // resetting the slash flag
			}

			if (command.at(i) == ';') {
				// to check if there is an escape command right after the ";", we will check if the substring from after the ";" to the end
				// of command starts with an escape command, but don't forget to deal with spaces/tabs in between
				string afterSemicolon = command.substr(i + 1);
				removeIndents(afterSemicolon);
				if (commandIs(afterSemicolon, "/escprint") || commandIs(afterSemicolon, "/escvarprint")) {
					return true;
				}
			}

		}
		return false;
	}
}

void removeIndents(string& command) {
	while (beginsWith(command, " ") || beginsWith(command, "\t")) {
		command = removeLeadingSpaces(command);
		command = removeLeadingCharacters(command, '\t');
	}
}

void removeInlineComments(string& command) {
	if (isEscapeCommand(command)) {
		return;
	}
	if (strUtil::contains(command, "//")) {
		int posInlineComment = command.find("//");
		command = command.substr(0, posInlineComment);
		while (endsWith(command, " ") || endsWith(command, "\t")) {
			command = strUtil::removeTrailingSpaces(command);
			command = strUtil::removeTrailingCharacters(command, '\t');
		}
	}
}

void spliceSemicolonCommands(string& command, vector<string>& commands, int spliceAtIndex) {
	if (commandIs(command, "/escprint") || commandIs(command, "/escvarprint")) {
		return; // this is okay, no need to use isEscapeCommand() since we don't need to check for ";" when trying to process it (lol)
	}
	if (strUtil::contains(command, ";")) {
		int posSemicolon = command.find(";");
		string otherPart = command.substr(posSemicolon + 1);
		command = command.substr(0, posSemicolon);
		while (endsWith(command, " ") || endsWith(command, "\t")) {
			command = strUtil::removeTrailingSpaces(command);
			command = strUtil::removeTrailingCharacters(command, '\t');
		}
		vecUtil::insertAtPos(commands, spliceAtIndex, otherPart);
	}
}

void lineContinuation(string& command, vector<string>& commands, int currIndex) {
	if (isEscapeCommand(command)) {
		return;
	}
	if (strUtil::endsWith(command, "...")) {
		string mainPart = command.substr(0, command.size() - 3); // the part of the command without the "..."
		while (endsWith(mainPart, " ") || endsWith(mainPart, "\t")) {
			mainPart = strUtil::removeTrailingSpaces(mainPart);
			mainPart = strUtil::removeTrailingCharacters(mainPart, '\t');
		}
		string nextCommand = commands.at(currIndex + 1);
		vecUtil::removeByIndex(commands, currIndex + 1);
		removeIndents(nextCommand);
		command = mainPart + " " + nextCommand; // mainPart has trailing spaces/tabs removed, and nextCommand has leading ones removed
	}
}

bool hasComment(const string& command) {
	if (isEscapeCommand(command)) {
		return false; // by definition, these commands will print the "..." or "//" instead of treating them like special characters
	} else {
		return strUtil::contains(command, "//");
	}
}

bool endsWithDots(const string& command) {
	if (isEscapeCommand(command)) {
		return false; // by definition, these commands will print the "..." or "//" instead of treating them like special characters
	} else {
		return strUtil::endsWith(command, "...");
	}
}

void preprocess(string& command, vector<string>& commands, int currIndex) {
	removeIndents(command);
	bool firstTime = true;
	while (firstTime || hasComment(command) || endsWithDots(command)) {
		removeInlineComments(command);
		lineContinuation(command, commands, currIndex);
		spliceSemicolonCommands(command, commands, currIndex + 1);
		firstTime = false;
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
			    "Use " << ANSI_YELLOW << "/store" << ANSI_RED << " with 3 arguments (including type) if you want to create a new variable.\n" << ANSI_NORMAL;
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
		cout << ANSI_RED << "There is currently no variable named \"" << varname << "\".\n" << ANSI_NORMAL;
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
		cout << ANSI_RED << "There is currently no variable named \"" << varname << "\".\n" << ANSI_NORMAL;
	}
}

void getType(const string& strvar, const string& varname) {
	if (!contains(vars, varname)) {
		cout << ANSI_RED << "There is currently no variable named \"" << varname << "\".\n" << ANSI_NORMAL;
		return;
	}
	string type = find(vars, varname).datatype;
	createVar(strvar, type, "String");
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
		cout << ANSI_YELLOW << "/ifvar" << ANSI_RED << " or " << ANSI_YELLOW << "/while" << ANSI_RED << " cannot execute because the boolean variable \"" << boolvarname << "\" is not found.\n" << ANSI_NORMAL;
		return;
	}
	Variable boolvar = find(vars, boolvarname);
	if (boolvar.datatype != "Bool") {
		cout << ANSI_YELLOW << "/ifvar" << ANSI_RED << " or " << ANSI_YELLOW << "/while" << ANSI_RED << " cannot execute because the variable \"" << boolvarname << "\" is not of type Bool.\n" << ANSI_NORMAL;
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
		cout << ANSI_YELLOW << "/for" << ANSI_RED << " cannot execute because the numerical variable \"" << numvarname << "\" is not found.\n" << ANSI_NORMAL;
		return;
	}
	Variable numvar = find(vars, numvarname);
	if (numvar.datatype != "Number") {
		cout << ANSI_YELLOW << "/for" << ANSI_RED << " cannot execute because the variable \"" << numvarname << "\" is not of type Number.\n" << ANSI_NORMAL;
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

void delStruct(const string& srtname) {
	srt::deleteStruct(structs, objects, vars, srtname);
}

void delObject(const string& objname) {
	srt::deleteObject(objects, vars, objname);
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

void existStruct(const string& boolvarname, const string& srtname) {
	bool exist = srt::containsStruct(structs, srtname);
	createVar(boolvarname, boolval(exist), "Bool");
}

void existObject(const string& boolvarname, const string& objname) {
	bool exist = srt::containsObject(objects, objname);
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

void customFail(const string& command, const string& message) {
	cout << ANSI_RED << "An assertion has failed! The assertion that caused this is: " << ANSI_YELLOW << command << ANSI_RED << ".\n"
	 	 << "Error message: " << ANSI_YELLOW << message << "\n" << ANSI_NORMAL;
	non_assert_crash = false;
	throw runtime_error("assertion failed");
}

void customAssert(const string& command, bool boolvalue, const string& message) {
	if (!boolvalue) {
		customFail(command, message);
	}
}

void customAssertVar(const string& command, const string& boolvarname, const string& message) {
	if (!contains(vars, boolvarname)) {
		cout << ANSI_RED << "Since the variable \"" << boolvarname << "\" does not currently exist, this assertion cannot proceed.\n"
			 << "The program will now attempt to continue execution.\n" << ANSI_NORMAL;
		return;
	}
	Variable boolvar = find(vars, boolvarname);
	if (boolvar.datatype != "Bool") {
		cout << ANSI_RED << "Since the type of the variable \"" << boolvarname << "\" is not \"Bool\", this assertion cannot proceed.\n"
			 << "The program will now attempt to continue execution.\n" << ANSI_NORMAL;
		return;
	}
	customAssert(command, boolvar.getBooleanValue(), message);
}

void customAssertType(const string& command, const string& varname, const string& type, const string& message) {
	if (!contains(vars, varname)) {
		cout << ANSI_RED << "Since the variable \"" << varname << "\" does not currently exist, this assertion cannot proceed.\n"
			 << "The program will now attempt to continue execution.\n" << ANSI_NORMAL;
		return;
	}
	string vartype = find(vars, varname).datatype;
	customAssert(command, vartype == type, message);
}

void customAssertObjectType(const string& command, const string& objname, const string& srtname, const string& message) {
	if (!containsObject(objects, objname)) {
		cout << ANSI_RED << "Since the object \"" << objname << "\" does not currently exist, this assertion cannot proceed.\n"
			 << "The program will now attempt to continue execution.\n" << ANSI_NORMAL;
		return;
	}
	if (!containsStruct(structs, srtname)) {
		cout << ANSI_RED << "Since the struct \"" << srtname << "\" does not currently exist, this assertion cannot proceed.\n"
			 << "The program will now attempt to continue execution.\n" << ANSI_NORMAL;
		return;
	}
	string structtype = findObject(objects, objname).structTypename;
	customAssert(command, structtype == srtname, message);
}

void structdef(const string& command) {
	string structName = parseArgument(command, 1);
	vector<string> fieldInfo;
	for (int i = 2; i <= numArguments(command); i++) {
		fieldInfo.push_back(parseArgument(command, i));
	}
	srt::attemptStructDefinition(structName, fieldInfo, structs, non_assert_crash);
}

void construct(const string& command) {
	string objectName = parseArgument(command, 1);
	string typeName = parseArgument(command, 2);
	if (!srt::containsStruct(structs, typeName)) {
		cout << ANSI_RED << "There is currently no struct named \"" << typeName << "\".\n" << ANSI_NORMAL;
		return;
	}
	vector<string> fieldInitValues;
	for (int i = 3; i <= numArguments(command); i++) {
		fieldInitValues.push_back(parseArgument(command, i));
	}
	srt::attemptObjectConstruction(objectName, srt::findStruct(structs, typeName), fieldInitValues, vars, objects, structs, non_assert_crash);
}

void setDefault(const string& command) {
	string srtname = parseArgument(command, 1);
	if (!srt::containsStruct(structs, srtname)) {
		cout << ANSI_RED << "There is currently no struct named \"" << srtname << "\".\n" << ANSI_NORMAL;
		return;
	}
	Struct& srt = findStruct(structs, srtname);
	for (int i = 2; i <= numArguments(command); i++) {
		srt.defaultValues.push_back(parseArgument(command, i));
	}
}

void consDefault(const string& command) {
	string objectName = parseArgument(command, 1);
	string typeName = parseArgument(command, 2);
	if (!srt::containsStruct(structs, typeName)) {
		cout << ANSI_RED << "There is currently no struct named \"" << typeName << "\".\n" << ANSI_NORMAL;
		return;
	}
	vector<string> defaultValues = findStruct(structs, typeName).defaultValues;
	if (defaultValues.empty()) {
		cout << ANSI_RED << "The struct \"" << typeName << "\" does not have a default constructor yet. "
			 << "Make one using the " << ANSI_YELLOW << "/setdefault" << ANSI_RED << " command first, "
			 << "before using " << ANSI_YELLOW << "/consdefault" << ANSI_RED << ".\n" << ANSI_NORMAL;
		return;
	}
	srt::attemptObjectConstruction(objectName, srt::findStruct(structs, typeName), defaultValues, vars, objects, structs, non_assert_crash);
}

void copyAssignment(const Object& sourceObject, const Object& destObject) {
	for (int i = 0; i < sourceObject.fieldnames.size(); i++) {
		string valueToCopy = var::find(vars, sourceObject.fieldnames.at(i)).value;
		var::find(vars, destObject.fieldnames.at(i)).value = valueToCopy;
	}
	// recursively copy inner objects
	for (int i = 0; i < sourceObject.objFieldnames.size(); i++) {
		Object& objectToCopySource = srt::findObject(objects, sourceObject.objFieldnames.at(i));
		Object& objectToCopyDest = srt::findObject(objects, destObject.objFieldnames.at(i));
		copyAssignment(objectToCopySource, objectToCopyDest);
	}
}

void nestedCopyConstruction(const Object& sourceNestedObject, vector<string>& fieldInitValues) {
	Struct& sourceNestedStruct = srt::findStruct(structs, sourceNestedObject.structTypename);
	int indexNestedPrimitive = 0;
	int indexDoublyNestedObject = 0;
	for (const pair<string, string>& nestedFieldAndType : sourceNestedStruct.fieldsAndTypes) {
		if (var::isPrimitive(nestedFieldAndType.second)) {
			fieldInitValues.push_back(var::find(vars, sourceNestedObject.fieldnames.at(indexNestedPrimitive)).value);
			indexNestedPrimitive++;
		} else {
			// recursion again!
			Object& doublyNestedObjectToCopy = srt::findObject(objects, sourceNestedObject.objFieldnames.at(indexDoublyNestedObject));
			nestedCopyConstruction(doublyNestedObjectToCopy, fieldInitValues);
			indexDoublyNestedObject++;
		}
	}
}

void copyConstruction(const Object& sourceObject, const string& destObjname) {
	vector<string> fieldInitValues;
	Struct& sourceStruct = srt::findStruct(structs, sourceObject.structTypename);
	int indexPrimitive = 0;
	int indexNestedObject = 0;
	for (const pair<string, string>& fieldAndType : sourceStruct.fieldsAndTypes) {
		if (var::isPrimitive(fieldAndType.second)) {
			fieldInitValues.push_back(var::find(vars, sourceObject.fieldnames.at(indexPrimitive)).value);
			indexPrimitive++;
		} else {
			Object& nestedObjectToCopy = srt::findObject(objects, sourceObject.objFieldnames.at(indexNestedObject));
			nestedCopyConstruction(nestedObjectToCopy, fieldInitValues);
			indexNestedObject++;
		}
	}
	srt::attemptObjectConstruction(destObjname, sourceStruct, fieldInitValues, vars, objects, structs, non_assert_crash);
}

void copyObject(const string& command) {
	string destObjname = parseArgument(command, 1);
	string sourceObjname = parseArgument(command, 2);
	if (!srt::containsObject(objects, sourceObjname)) {
		cout << ANSI_RED << "There is currently no source object named \"" << sourceObjname << "\" to copy from.\n" << ANSI_NORMAL;
		return;
	}
	Object& sourceObject = findObject(objects, sourceObjname);
	if (srt::containsObject(objects, destObjname)) {
		// copy assignment
		Object& destObject = findObject(objects, destObjname);
		if (destObject.structTypename != sourceObject.structTypename) {
			cout << ANSI_RED << "The type of dest object (" << destObject.structTypename << ") is not the same as the type of source object (" << sourceObject.structTypename << ").\n" << ANSI_NORMAL;
			return;
		}
		copyAssignment(sourceObject, destObject);
	} else {
		// copy constructor
		copyConstruction(sourceObject, destObjname);
	}
}

void getObjectType(const string& strvar, const string& objname) {
	if (!containsObject(objects, objname)) {
		cout << ANSI_RED << "There is currently no object named \"" << objname << "\".\n" << ANSI_NORMAL;
		return;
	}
	string type = findObject(objects, objname).structTypename;
	createVar(strvar, type, "String");
}

void inherit(const string& command) {
	string subsrtname = parseArgument(command, 1);
	string supersrtname = parseArgument(command, 2);
	if (!containsStruct(structs, supersrtname)) {
		cout << ANSI_RED << "There is currently no super-struct named \"" << supersrtname << "\" to inherit from.\n" << ANSI_NORMAL;
		return;
	}
	Struct& superstruct = findStruct(structs, supersrtname);
	vector<string> fieldInfo;
	for (const pair<string, string>& fieldAndType : superstruct.fieldsAndTypes) {
		fieldInfo.push_back(fieldAndType.first);
		fieldInfo.push_back(fieldAndType.second);
	}
	for (int i = 3; i <= numArguments(command); i++) {
		fieldInfo.push_back(parseArgument(command, i));
	}
	srt::attemptStructDefinition(subsrtname, fieldInfo, structs, non_assert_crash);
}

void stringRep(const string& srtname, const string& rep) {
	if (!containsStruct(structs, srtname)) {
		cout << ANSI_RED << "There is currently no struct named \"" << srtname << "\".\n" << ANSI_NORMAL;
		return;
	}
	Struct& srt = findStruct(structs, srtname);
	vector<string> parts = strUtil::partsSplitByOpenCloseDelimiters(rep, '<', '>');
	for (const string& part : parts) {
		if (strUtil::contains(part, "<")) { // has angle brackets
			string fieldname = part.substr(1, part.size() - 2); // removes angle brackets
			if (!vecUtil::contains(srt.getFields(), fieldname)) {
				cout << ANSI_RED << "The struct \"" << srt.name << "\" does not have a field named \"" << fieldname << "\".\n" << ANSI_NORMAL;
				return;
			}
		}
	}
	srt.rep = rep;
}