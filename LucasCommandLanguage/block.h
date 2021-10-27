#ifndef BLOCK_DOT_H
#define BLOCK_DOT_H

#include "stringUtility.h"
#include "vectorUtility.h"

using namespace std;

namespace blk {

	const string TEMP_BLOCK_PREFIX = "__temp__"; // reserved block name prefix for the interpreter to use

	struct Block; // forward declaration of this struct
	string uniqueNumber(const vector<Block>& blocks, const string& nameAllBeforeFinalUnderscore); // prototype for use later

	struct Block {

		string name;
		vector<string> commands;

		Block(const string& n) {
			name = n;
			commands = {};
		}

		Block(const string& n, const vector<string>& v) {
			name = n;
			commands = v;
		}

		void add(const string& com) {
			commands.push_back(com);
		}

		bool empty() const {
			return commands.empty();
		}

		void spliceInto(vector<string>& commands, int currIndex) const {
			vecUtil::spliceAtPos(commands, currIndex, this->commands);
		}

		// see cflowWhile() in Interpreter.cpp for details
		string createTemp(vector<Block>& blocks, const string& condvarname) const {
			string mainName = TEMP_BLOCK_PREFIX + this->name;
			string newName = mainName + "_" + uniqueNumber(blocks, mainName);
			Block newBlock(newName);
			newBlock.add(string() + "/block " + this->name);
			newBlock.add(string() + "/ifvar " + condvarname + " " + newName);
			blocks.push_back(newBlock);
			return newName;
		}

	};

	bool sameExceptForNumber(const string& blockname, const string& nameAllBeforeFinalUnderscore) {
		if (!strUtil::contains(blockname, "_")) {
			return false; // because nameAllBeforeFinalUnderscore is guaranteed to have underscores (by definition)
		}
		int posLastUnderscore = strUtil::positionOfFinalOccurrence(blockname, '_');
		string sub = blockname.substr(0, posLastUnderscore);
		return sub == nameAllBeforeFinalUnderscore;
	}

	// used to prevent different blocks from having the same names
	string uniqueNumber(const vector<Block>& blocks, const string& nameAllBeforeFinalUnderscore) {
		int count = 0;
		for (const Block& block : blocks) {
			if (sameExceptForNumber(block.name, nameAllBeforeFinalUnderscore)) {
				count++;
			}
		}
		return to_string(count); // built-in to_string for integers
	}

	Block& find(vector<Block>& v, const string& blockname) {
		for (Block& b : v) {
			if (b.name == blockname) {
				return b;
			}
		}
		throw runtime_error("block not found");
	}

	ostream& operator << (ostream& out, const Block& block) {
		using vecUtil::operator<<; // so I can use the << operator on block.commands
		out << "name: " << block.name << " & commands: " << block.commands;
		return out;
	}

}

#endif