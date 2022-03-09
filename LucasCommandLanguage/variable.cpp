#include "variable.h"

var::Variable::Variable(const string& varname, double data) {
	name = varname;
	value = numUtil::doubleToString(data);
	datatype = "Number";
}

var::Variable::Variable(const string& varname, const string& data) {
	name = varname;
	value = data;
	datatype = "String";
}

var::Variable::Variable(const string& varname, bool data) {
	name = varname;
	value = strUtil::boolval(data);
	datatype = "Bool";
}

var::Variable::Variable(const string& varname, const string& data, const string& type) {
	name = varname;
	value = data;
	datatype = type;
}

string var::Variable::getStringValue() const {
	return value;
}

double var::Variable::getNumericalValue() const {
	return stod(value);
}

bool var::Variable::getBooleanValue() const {
	return strUtil::parseBool(value);
}

string var::Variable::getAppropriateValue() const {
	if (datatype == "Number") {
		return numUtil::toStringRemoveTrailingZeros(getNumericalValue());
	} else if (datatype == "Bool") {
		return strUtil::boolval(getBooleanValue());
	} else if (datatype == "String") {
		return getStringValue();
	} else {
		throw runtime_error("invalid primitive data type");
	}
}

bool var::operator == (const Variable& v1, const Variable& v2) {
	return tie(v1.name, v1.value, v1.datatype) == tie(v2.name, v2.value, v2.datatype);
}

bool var::operator != (const Variable& v1, const Variable& v2) {
	return !(v1 == v2);
}

bool var::contains(const vector<Variable>& v, const string& varname) {
	for (int i = v.size() - 1; i >= 0; i--) {
		if (v.at(i).name == varname) {
			return true;
		}
	}
	return false;
}

var::Variable& var::find(vector<Variable>& v, const string& varname) {
	for (int i = v.size() - 1; i >= 0; i--) {
		if (v.at(i).name == varname) {
			return v.at(i);
		}
	}
	throw runtime_error("no matching variable");
}

void var::remove(vector<Variable>& v, const string& varname) {
	if (!contains(v, varname)) {
		return;
	}
	Variable var = find(v, varname);
	vecUtil::removeFirstInstance(v, var);
}

ostream& var::operator << (ostream& out, const Variable& var) {
	out << "name: " << var.name << " & value: ";
	if (var.datatype == "String" || var.datatype == "Bool") {
		out << var.value;
	} else if (var.datatype == "Number") {
		out << stod(var.value);
	}
	out << " & type: " << var.datatype;
	return out;
}

void var::replaceVariableReferencesWithRoundedValues(string& s, vector<Variable>& vars, int numPlaces) {
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
				newString += numUtil::roundToNplaces(var.value, numPlaces);
			}
		}
	}
	s = newString;
}

void var::replaceVariableReferencesWithFullPrecisionValues(string& s, vector<Variable>& vars) {
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

bool var::isPrimitive(const string& type) {
	return type == "Number" || type == "Bool" || type == "String";
}