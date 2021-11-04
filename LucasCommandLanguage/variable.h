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

		Variable(const string& varname, double data) {
			name = varname;
			value = numUtil::doubleToString(data);
			datatype = "Number";
		}

		Variable(const string& varname, const string& data) {
			name = varname;
			value = data;
			datatype = "String";
		}

		Variable(const string& varname, bool data) {
			name = varname;
			value = strUtil::boolval(data);
			datatype = "Bool";
		}

		Variable(const string& varname, const string& data, const string& type) {
			name = varname;
			value = data;
			datatype = type;
		}

		string getStringValue() const {
			return value;
		}

		double getNumericalValue() const {
			return stod(value);
		}

		bool getBooleanValue() const {
			return strUtil::parseBool(value);
		}

	};

	bool operator == (const Variable& v1, const Variable& v2) {
		return tie(v1.name, v1.value, v1.datatype) == tie(v2.name, v2.value, v2.datatype);
	}

	bool operator != (const Variable& v1, const Variable& v2) {
		return !(v1 == v2);
	}

	bool contains(const vector<Variable>& v, const string& varname) {
		for (int i = v.size() - 1; i >= 0; i--) {
			if (v.at(i).name == varname) {
				return true;
			}
		}
		return false;
	}

	Variable& find(vector<Variable>& v, const string& varname) {
		for (int i = v.size() - 1; i >= 0; i--) {
			if (v.at(i).name == varname) {
				return v.at(i);
			}
		}
		throw runtime_error("no matching variable");
	}

	void remove(vector<Variable>& v, const string& varname) {
		if (!contains(v, varname)) {
			return;
		}
		Variable var = find(v, varname);
		vecUtil::removeFirstInstance(v, var);
	}

	ostream& operator << (ostream& out, const Variable& var) {
		out << "name: " << var.name << " & value: ";
		if (var.datatype == "String" || var.datatype == "Bool") {
			out << var.value;
		} else if (var.datatype == "Number") {
			out << stod(var.value);
		}
		out << " & type: " << var.datatype;
		return out;
	}

	void replaceVariableReferencesWithRoundedValues(string& s, vector<Variable>& vars, int numPlaces) {
		vector<string> parts = strUtil::partsSplitByOpenCloseDelimiters(s, '{', '}');
		string newString;
		for (const string& part : parts) {
			if (!strUtil::contains(part, "{")) {
				// this part has no braces
				newString += part;
			} else {
				// this part has braces
				string noBraces = part.substr(1, part.size() - 2); // removes opening and closing braces
				if (!contains(vars, noBraces)) {
					cout << ANSI_RED << "There is currently no variable named \"" << noBraces << "\"\n" << ANSI_NORMAL;
					s = "";
					return;
				}
				Variable var = find(vars, noBraces);
				if (var.datatype == "String" || var.datatype == "Bool") {
					newString += var.value;
				} else if (var.datatype == "Number") {
					newString += numUtil::toStringRemoveTrailingZeros(numUtil::round(var.getNumericalValue(), numPlaces));
				}
			}
		}
		s = newString;
	}

	void replaceVariableReferencesWithFullPrecisionValues(string& s, vector<Variable>& vars) {
		vector<string> parts = strUtil::partsSplitByOpenCloseDelimiters(s, '[', ']');
		string newString;
		for (const string& part : parts) {
			if (!strUtil::contains(part, "[")) {
				// this part has no square brackets
				newString += part;
			} else {
				// this part has square brackets
				string noSquareBrackets = part.substr(1, part.size() - 2); // removes opening and closing square brackets
				if (!contains(vars, noSquareBrackets)) {
					cout << ANSI_RED << "There is currently no variable named \"" << noSquareBrackets << "\"\n" << ANSI_NORMAL;
					s = "";
					return;
				}
				Variable var = find(vars, noSquareBrackets);
				newString += var.value;
			}
		}
		s = newString;
	}

}

#endif