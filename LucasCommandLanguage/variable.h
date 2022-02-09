#ifndef VARIABLE_DOT_H
#define VARIABLE_DOT_H

#include <vector>
#include <string>
#include <tuple>
#include <stdexcept>

#include "ansi_codes.h"
#include "stringUtility.h"
#include "numberUtility.h"

using namespace std;

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

	};

	bool operator == (const Variable& v1, const Variable& v2);
	bool operator != (const Variable& v1, const Variable& v2);
	bool contains(const vector<Variable>& v, const string& varname);
	Variable& find(vector<Variable>& v, const string& varname);
	void remove(vector<Variable>& v, const string& varname);
	ostream& operator << (ostream& out, const Variable& var);
	void replaceVariableReferencesWithRoundedValues(string& s, vector<Variable>& vars, int numPlaces);
	void replaceVariableReferencesWithFullPrecisionValues(string& s, vector<Variable>& vars);

}

#endif