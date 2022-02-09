#include "block.h"

blk::Block::Block(const string& n) {
	name = n;
	commands = {};
}

blk::Block::Block(const string& n, const vector<string>& v) {
	name = n;
	commands = v;
}

void blk::Block::add(const string& com) {
	commands.push_back(com);
}

bool blk::Block::empty() const {
	return commands.empty();
}

void blk::Block::spliceInto(vector<string>& commands, int currIndex) const {
	vecUtil::spliceAtPos(commands, currIndex, this->commands);
}

// see cflowWhile() in Interpreter.cpp for details
string blk::Block::createTemp(vector<Block>& blocks, const string& condvarname) const {
	string mainName = TEMP_BLOCK_PREFIX + this->name;
	string newName = mainName + "_" + uniqueNumber(blocks, mainName);
	Block newBlock(newName);
	newBlock.add(string() + "/block " + this->name);
	newBlock.add(string() + "/ifvar " + condvarname + " " + newName);
	blocks.push_back(newBlock);
	return newName;
}

bool blk::operator == (const Block& block1, const Block& block2) {
	return tie(block1.name, block1.commands) == tie(block2.name, block2.commands);
}

bool blk::operator != (const Block& block1, const Block& block2) {
	return !(block1 == block2);
}

bool blk::sameExceptForNumber(const string& blockname, const string& nameAllBeforeFinalUnderscore) {
	if (!strUtil::contains(blockname, "_")) {
		return false; // because nameAllBeforeFinalUnderscore is guaranteed to have underscores (by definition)
	}
	int posLastUnderscore = strUtil::positionOfFinalOccurrence(blockname, '_');
	string sub = blockname.substr(0, posLastUnderscore);
	return sub == nameAllBeforeFinalUnderscore;
}

// used to prevent different blocks from having the same names
string blk::uniqueNumber(const vector<Block>& blocks, const string& nameAllBeforeFinalUnderscore) {
	int count = 0;
	for (const Block& block : blocks) {
		if (sameExceptForNumber(block.name, nameAllBeforeFinalUnderscore)) {
			count++;
		}
	}
	return to_string(count); // built-in to_string for integers
}

bool blk::contains(const vector<Block>& v, const string& blockname) {
	for (int i = v.size() - 1; i >= 0; i--) {
		if (v.at(i).name == blockname) {
			return true;
		}
	}
	return false;
}

blk::Block& blk::find(vector<Block>& v, const string& blockname) {
	for (int i = v.size() - 1; i >= 0; i--) {
		if (v.at(i).name == blockname) {
			return v.at(i);
		}
	}
	throw runtime_error("block not found");
}

void blk::remove(vector<Block>& v, const string& blockname) {
	if (!contains(v, blockname)) {
		return;
	}
	Block block = find(v, blockname);
	vecUtil::removeFirstInstance(v, block);
}

ostream& blk::operator << (ostream& out, const Block& block) {
	using vecUtil::operator<<; // so I can use the << operator on block.commands
	out << "name: " << block.name << " & commands: " << block.commands;
	return out;
}