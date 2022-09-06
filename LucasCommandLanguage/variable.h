#ifndef VARIABLE_DOT_H
#define VARIABLE_DOT_H

#include <vector>
#include <string>
#include <tuple>
#include <stdexcept>

#include "general/ansi_codes.h"
#include "general/stringUtility.h"
#include "general/numberUtility.h"

#include "struct.h"

using namespace std;

namespace srt {
	struct Struct; // definition found in "struct.h"
	struct Object; // definition found in "struct.h"
}

namespace var {

	struct Variable {

		string name;
		string value;
		string datatype;

		Variable(const string& varname, double data);
		Variable(const string& varname, const string& data);
		Variable(const string& varname, bool data);
		Variable(const string& varname, const string& data, const string& type);

		string getStringValue() const;
		double getNumericalValue() const;
		bool getBooleanValue() const;
		string getAppropriateValue() const;

		// the parameter numPlaces is greater or equal to 0 if we should round, and is equal to -1 if we should NOT round
		bool equals(const Variable& other, int numPlaces) const;

	};

	bool operator == (const Variable& v1, const Variable& v2);
	bool operator != (const Variable& v1, const Variable& v2);
	bool contains(const vector<Variable>& v, const string& varname);
	Variable& find(vector<Variable>& v, const string& varname);
	void remove(vector<Variable>& v, const string& varname);
	ostream& operator << (ostream& out, const Variable& var);
	void replaceVariableReferencesWithRoundedValues(string& s, vector<Variable>& vars,
													vector<srt::Object>& objects, vector<srt::Struct>& structs, int numPlaces);
	void replaceVariableReferencesWithFullPrecisionValues(string& s, vector<Variable>& vars,
														  vector<srt::Object>& objects, vector<srt::Struct>& structs);
	bool isPrimitive(const string& type);

}

#endif