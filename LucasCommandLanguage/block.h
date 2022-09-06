#ifndef BLOCK_DOT_H
#define BLOCK_DOT_H

#include "general/stringUtility.h"
#include "general/vectorUtility.h"

#include <tuple>

using namespace std;

namespace blk {

	const string TEMP_BLOCK_PREFIX = "__temp__"; // reserved block name prefix for the interpreter to use

	struct Block {

		string name;
		vector<string> commands;

		Block(const string& n);
		Block(const string& n, const vector<string>& v);

		void add(const string& com);
		bool empty() const;
		void spliceInto(vector<string>& commands, int currIndex) const;
		// see cflowWhile() in Interpreter.cpp for details
		string createTemp(vector<Block>& blocks, const string& condvarname) const;

	};

	bool operator == (const Block& block1, const Block& block2);
	bool operator != (const Block& block1, const Block& block2);
	bool sameExceptForNumber(const string& blockname, const string& nameAllBeforeFinalUnderscore);
	// used to prevent different blocks from having the same names
	string uniqueNumber(const vector<Block>& blocks, const string& nameAllBeforeFinalUnderscore);
	bool contains(const vector<Block>& v, const string& blockname);
	Block& find(vector<Block>& v, const string& blockname);
	void remove(vector<Block>& v, const string& blockname);
	ostream& operator << (ostream& out, const Block& block);

}

#endif