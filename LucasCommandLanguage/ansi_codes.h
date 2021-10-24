/*
 * File:   ansi_codes.h
 * Author: Brian Fraser
 *
 * Contains the ANSI codes which change the font colour and style.
 * Usage:
 *
 *   1: Copy this file into the same folder as your project.
 *
 *   2: Include this file into your C++ file:
 *      #include "ansi_codes.h"
 *
 *   3: Use the ANSI_... constants defined below in cout statements, such as:
 *      cout << "Hello " << ANSI_RED << "world" << ANSI_NORMAL << "!\n";
 *      cout << ANSI_BOLD << ANSI_GREEN << "test" << ANSI_NORMAL << endl;
 */

#ifndef ANSI_CODES_H
#define	ANSI_CODES_H

#include <string>
using namespace std;


// ANSI screen colour codes
// Source: http://en.wikipedia.org/wiki/ANSI_escape_code
const string ANSI_NORMAL = "\033[0m";

const string ANSI_BLACK = "\033[30m";
const string ANSI_RED = "\033[31m";
const string ANSI_GREEN = "\033[32m";
const string ANSI_YELLOW = "\033[33m";
const string ANSI_BLUE = "\033[34m";
const string ANSI_MAGENTA = "\033[35m";
const string ANSI_CYAN = "\033[36m";
const string ANSI_WHITE = "\033[37m";

const string ANSI_BACKGROUND_BLACK = "\033[40m";
const string ANSI_BACKGROUND_RED = "\033[41m";
const string ANSI_BACKGROUND_GREEN = "\033[42m";
const string ANSI_BACKGROUND_YELLOW = "\033[43m";
const string ANSI_BACKGROUND_BLUE = "\033[44m";
const string ANSI_BACKGROUND_MAGENTA = "\033[45m";
const string ANSI_BACKGROUND_CYAN = "\033[46m";
const string ANSI_BACKGROUND_WHITE = "\033[47m";

const string ANSI_UNDERLINE = "\033[4m";
const string ANSI_BOLD = "\033[1m";
const string ANSI_ITALIC = "\033[3m";
const string ANSI_INVERSE = "\033[7m"; // Swap foreground & background colours
const string ANSI_STRIKETHROUGH = "\033[9m";


// DEMO CODE
// Copy and paste the following code into your program to have it show you all
// the colours and styles. Note that __LINE__ is the current line number
// of the file (which helps you figure out which colour/style you applied).
/*
	cout << "\nNORMAL COLOURS\n";
	cout << ANSI_BLACK << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_RED << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_GREEN << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_YELLOW << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_BLUE << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_MAGENTA << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_CYAN << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_WHITE << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;

	cout << "\nBOLD STYLE\n";
	cout << ANSI_BOLD << ANSI_BLACK << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_BOLD << ANSI_RED << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_BOLD << ANSI_GREEN << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_BOLD << ANSI_YELLOW << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_BOLD << ANSI_BLUE << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_BOLD << ANSI_MAGENTA << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_BOLD << ANSI_CYAN << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_BOLD << ANSI_WHITE << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;

	cout << "\nBACKGROUND COLOURS\n";
	cout << ANSI_BACKGROUND_BLACK << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_BACKGROUND_RED << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_BACKGROUND_GREEN << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_BACKGROUND_YELLOW << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_BACKGROUND_BLUE << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_BACKGROUND_MAGENTA << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_BACKGROUND_CYAN << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_BACKGROUND_WHITE << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;

	cout << "\nSTYLES\n";
	cout << ANSI_BOLD << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_ITALIC << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_UNDERLINE << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_INVERSE << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
	cout << ANSI_STRIKETHROUGH << "Testing @ line " << __LINE__ << ANSI_NORMAL << endl;
 */

#endif	/* ANSI_CODES_H */