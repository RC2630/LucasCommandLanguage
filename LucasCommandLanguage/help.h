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
		"/help blocks = display help document for blocks (how to use blocks, including examples)\n"
		"/help memory = display help document for memory management and cleaning\n"
		"/help lclinfo = display information on this programming language (LCL = Lucas Command Language), including copyright";

	const string COMMAND_HELP_DIRECTORY =

		"LIST OF HELP COMMANDS TO FOR ALL CATEGORIES OF COMMANDS:\n\n"

		"/help commands general = display list of general commands\n"
		"/help commands variables = display commands related to variables\n"
		"/help commands arithmetic = display commands related to arithmetic\n"
		"/help commands relational = display commands that compare numbers\n"
		"/help commands logical = display commands that operate on booleans\n"
		"/help commands cast = display commands that convert/cast variables to a different type\n"
		"/help commands string = display commands that operate on strings\n"
		"/help commands blocks = display commands related to blocks\n"
		"/help commands cflow = display commands related to control flow\n"
		"/help commands memory = display commands related to memory";

	const string COMMAND_HELP_GENERAL =

		"LIST OF GENERAL COMMANDS:\n\n"

		"<empty line> = absolutely nothing (just to make source code look nicer)\n"
		"<any non-empty line that doesn't start with \"/\"> = display the line verbatim\n"
		"// <anything> = just a comment (the space between // and comment is not required, but it looks nicer with the space)\n\n"

		"/help = display help directory (for details on specific help commands, type /help)\n"
		"/stop = terminate program\n"
		"/prev = reuse previous command\n\n"

		"/line = print 1 newline character\n"
		"/line <n> = print n copies of the newline character\n"
		"/digits <n> = use n decimal places for the display of all numerical types (0 <= n <= 12), note that calculations still use much higher precision than display though\n\n"

		"/warntype <b> = set whether or not changing the type of a variable will display a warning message\n"
		"/useblue <b> = set whether or not displaying verbatim lines will be in blue (if false, then it will be in the default colour)\n"
		"/reusedisp <b> = set whether or not /prev will print reuse message whenever it is used\n\n"
		
		"/escprint <escmessage> = display escmessage verbatim, escaping all instances of \"/\", \"[]\", \"{}\", etc. (variables will NOT be replaced with their values)\n"
		"/escvarprint <escvarmessage> = display escvarmessage verbatim, escaping all instances of \"/\", \"[]\", etc. but NOT \"{}\" (allowing for curly-brace-accessed variables to still work)\n"
		"/input <varname> <typename> <prompt> = display prompt, then wait for console input; this input is then stored into variable called varname with datatype typename (the input must not include any spaces)";

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

	const string COMMAND_HELP_RELATIONAL =

		"LIST OF RELATIONAL COMMANDS:\n\n"

		"/less <varname> <n1> <n2> ... <ni> = check each pair n[k-1] < n[k] for all k in {2, ..., i}, store true in varname if all such comparisons hold true, otherwise store false\n"
		"/greater <varname> <n1> <n2> ... <ni> = check each pair n[k-1] > n[k] for all k in {2, ..., i}, store true in varname if all such comparisons hold true, otherwise store false\n"
		"/le <varname> <n1> <n2> ... <ni> = check each pair n[k-1] <= n[k] for all k in {2, ..., i}, store true in varname if all such comparisons hold true, otherwise store false\n"
		"/ge <varname> <n1> <n2> ... <ni> = check each pair n[k-1] >= n[k] for all k in {2, ..., i}, store true in varname if all such comparisons hold true, otherwise store false\n"
		"/equal <varname> <n1> <n2> ... <ni> = store true in varname if all numerical arguments {n1, ..., ni} are equal, otherwise store false\n"
		"/notequal <varname> <n1> <n2> ... <ni> = store true in varname if at least 1 numerical argument in {n1, ..., ni} is not equal to another, otherwise store false";

	const string COMMAND_HELP_LOGICAL =

		"LIST OF LOGICAL COMMANDS:\n\n"

		"/and <varname> <b1> <b2> ... <bi> = evaluate b1 AND b2 AND ... AND bi, then store the result into variable called varname\n"
		"/or <varname> <b1> <b2> ... <bi> = evaluate b1 OR b2 OR ... OR bi, then store the result into variable called varname\n"
		"/xor <varname> <b1> <b2> = evaluate b1 XOR b2, then store the result into variable called varname\n"
		"/boolequal <varname> <b1> <b2> ... <bi> = store true in varname if all boolean arguments {b1, ..., bi} are equal, otherwise store false\n"
		"/not <varname> <b> = evaluate NOT b, then store the result into variable called varname";

	const string COMMAND_HELP_CAST =

		"LIST OF CASTING/CONVERSION COMMANDS:\n\n"

		"/numtobool <boolvar> <num> = if num is 0, store false into boolvar; otherwise, store true\n"
		"/booltonum <numvar> <bool> = if bool is false, store 0 into numvar; if bool is true, store 1\n"
		"/numtostr <strvar> <num> = convert num to a string, then store it in strvar\n"
		"/strtonum <numvar> <str> = convert str to a number if str is a valid string representation of a number and store it in numvar; if str does not represent a valid number, it is undefined behaviour\n"
		"/booltostr <strvar> <bool> = if bool is false, store false (as a string) into strvar; if bool is true, store true (as a string) into strvar\n"
		"/strtobool <boolvar> <str> = if str is one of {true, True, TRUE}, store true (as a bool) into boolvar; otherwise, store false (as a bool) into boolvar";

	const string COMMAND_HELP_STRING =

		"LIST OF STRING-RELATED COMMANDS:\n\n"

		"/length <varname> <s> = store the length of s into varname\n"
		"/contains <varname> <s1> <s2> = store true into varname if s2 is a substring of s1, store false otherwise\n"
		"/charat <varname> <s> <i> = store the i-th character of s into varname (index is 0-based)\n"
		"/concat <varname> <s1> <s2> ... <si> = concatenate all strings {s1, ..., si}, then store the result into varname\n"
		"/substr <varname> <s> <begin> <end> = find the substring of s from index begin to index end (inclusive, 0-based), then store the result into varname\n"
		"/strequal <varname> <s1> <s2> ... <si> = store true in varname if all string arguments {s1, ..., si} are equal, otherwise store false";

	const string COMMAND_HELP_BLOCKS =

		"LIST OF BLOCK-RELATED COMMANDS:\n\n"

		"/blockdef <blockname> = start the definition for a new block named blockname\n"
		"/endblock = end the definition for the current block\n"
		"/block <blockname> = execute the block named blockname\n\n"

		"Type \"/help blocks\" for more details on how to use blocks";

	const string COMMAND_HELP_CONTROL_FLOW =

		"LIST OF CONTROL-FLOW-RELATED COMMANDS:\n\n"
		
		"/if <b> <blockname> = run the block named blockname if b is true\n"
		"/ifvar <boolvar> <blockname> = run the block named blockname if boolvar holds value of true\n\n"

		"/loop <n> <blockname> = run the block named blockname n times\n"
		"/for <numvar> <blockname> = run the block named blockname n times, where n is the number stored by numvar when this command is executed (regardless of if numvar is changed later)\n"
		"/while <condvar> <blockname> = run the block named blockname while condvar holds value of true (it is intended that the value of condvar will change through the iterations)";

	const string COMMAND_HELP_MEMORY =

		"LIST OF MEMORY-RELATED COMMANDS:\n\n"

		"/delvar <varname> = delete variable named varname if such variable exists, otherwise do nothing\n"
		"/delblock <blockname> = delete block named blockname if such block exists, otherwise do nothing\n"
		"/clean = clean up all temporary blocks (these get created when \"/while\" is used); if there are no temporary blocks, this command does nothing\n\n"
		
		"/existvar <boolvar> <varname> = checks if variable named varname exists, and stores result into boolean variable named boolvar\n"
		"/existblock <boolvar> <blockname> = checks if block named blockname exists, and stores result into boolean variable named boolvar\n"
		"/existtemps <boolvar> = checks if there are any temporary blocks in the program, and stores result into boolean variable named boolvar\n\n"
		
		"Type \"/help memory\" for information on when and why you should use the deletion/cleaning commands";

	const string VARIABLE_HELP =
		
		"HELP WITH VARIABLES:\n\n"

		"Variables can be one of three types (Number, String, Bool) and can be named anything alpha-numeric (including underscores).\n"
		"For the purposes of this document:\n"
		"Variables of type Number are called numerical variables, variables of type String are called string variables, and variables of type Bool are called boolean variables.\n\n"

		"String variables do not have surrounding quotes.\n"
		"Right now, Strings with spaces in them are not supported - your String must not have any spaces.\n"
		"Boolean variables must be either \"true\" or \"false\" (without the quotes).\n\n"

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

		"Technically, variables can be used to store commands as well - simply store the command as a String.\n"
		"However, the no-space rule still applies, so any commands stored as variables cannot have spaces in them.\n"
		"Because of this, it is not recommended to store commands as variables.\n"
		"Instead, you should create blocks to store commands, as they are much more effective.\n"
		"Advantages of blocks over variables in command storage include the ability to store commands with spaces, as well as multiple commands (in the same block).\n"
		"Despite this, command-storing variables are still supported, so any String variable beginning with the '/' character will be treated as a command when used at the BEGINNING of a line in the source code.\n"
		"At the beginning of a line in the source code, if you want to display command-storing variables verbatim instead of run the command, use \"/escvarprint\" (use braces, not square brackets, to access the variable).\n"
		"Elsewhere in the source code, if you want to do the above, then no special measure needs to be taken (because the '/' character only has special significance at the beginning of lines in the source code).\n\n"
		
		"Type \"/help commands variables\" for a list of commands that create, modify, and access variables";

	const string BLOCK_HELP =

		"HELP WITH BLOCKS:\n\n"

		"Start the definition of a block with \"/blockdef <blockname>\" and end the definition with \"/endblock\".\n"
		"Between these two commands, you can place any number of other commands; these form the body of the block.\n"
		"The commands you put in the middle will not be executed until you explicitly run the block using \"/block <blockname>\".\n\n"

		"Note that you should not define any other blocks inside the definition for a block (no nested block definitions).\n"
		"You should also not create multiple blocks with the same block name - doing so is undefined behaviour.\n"
		"Also, attempting to run undefined blocks is undefined behaviour as well (unlike undefined variables, which display a warning message).\n\n"

		"Block names should be alpha-numeric (including underscores). Follow a consistent style/convention for naming blocks to distinguish them from variable names.\n"
		"All block names cannot begin with \"__temp__\" (2 underscores before, 2 underscores after). This name has special significance for the interpreter.\n\n"

		"Type \"/help commands blocks\" for a list of commands related to blocks";

	const string MEM_HELP =

		"HELP WITH MEMORY:\n\n"
		
		"As the number of variables and blocks get larger and larger, some of them generally do not need to be used/reused anymore.\n"
		"Due to this, it is good practice to explicitly delete variables and blocks that are no longer used afterwards.\n"
		"Doing so not only signifies to the programmer that the variable/block no longer exists, but also saves memory and makes the program run faster.\n"
		"However, having said so, you should ONLY ever delete a variable or block if you are 100% SURE that you don't need it anymore, since deletion is irreversible.\n\n"
		
		"A special case of block deletion arises when you use \"/while\".\n"
		"In LCL, while loops are implemented in such a way that additional, temporary blocks are required for the loop to work properly.\n"
		"However, these temporary blocks do not automatically get deleted at the end of the while loop (for complicated but valid reasons).\n"
		"Therefore, the programmer needs to explicitly delete these blocks when they start to accumulate in problematically large numbers.\n"
		"Note that it is not necessary to explicitly delete these blocks after EVERY while loop, just once in a while.\n"
		"However, after an especially lengthy while loop, it is good practice to delete these blocks.\n"
		"Other loops (such as \"/for\" and \"/loop\"), as well as recursive calls to blocks, do NOT require this type of deletion.\n\n"

		"WARNING: If there are nested while loops, do NOT explicitly delete temporary blocks, since doing so deletes all the temporary blocks.\n"
		"For example, if there is an outer while loop and an inner while loop, even if you intend to only clean up the temporary blocks left by the inner one,\n"
		"explicit deletion will cause BOTH the inner and outer loops' temporary blocks to be deleted.\n"
		"Therefore, wait until all layers of the nested loop are finished before performing the deletion.\n\n"

		"Type \"/help commands memory\" for a list of commands that perform these deletion operations";

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