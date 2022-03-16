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

// the parameter numPlaces is greater or equal to 0 if we should round, and is equal to -1 if we should NOT round
bool var::Variable::equals(const Variable& other, int numPlaces) const {
	if (this->datatype != other.datatype) {
		return false; // if they are not the same type, they cannot be equal
	}
	if (this->datatype == "String" || this->datatype == "Bool") {
		return this->getAppropriateValue() == other.getAppropriateValue();
	} else if (this->datatype == "Number") {
		if (numPlaces == -1) { // no rounding
			return this->getNumericalValue() == other.getNumericalValue();
		} else { // yes rounding
			return numUtil::roundToNplaces(this->value, numPlaces) == numUtil::roundToNplaces(other.value, numPlaces);
		}
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

void var::replaceVariableReferencesWithRoundedValues(string& s, vector<Variable>& vars,
													 vector<srt::Object>& objects, vector<srt::Struct>& structs, int numPlaces) {
	vector<string> parts = strUtil::partsSplitByOpenCloseDelimiters(s, '{', '}');
	string newString;
	for (const string& part : parts) {
		if (!strUtil::contains(part, "{")) {
			// this part has no braces
			newString += part;
		} else {
			// this part has braces
			string noBraces = part.substr(1, part.size() - 2); // removes opening and closing braces
			if (var::contains(vars, noBraces)) {
				Variable& var = find(vars, noBraces);
				if (var.datatype == "String" || var.datatype == "Bool") {
					newString += var.value;
				} else if (var.datatype == "Number") {
					newString += numUtil::roundToNplaces(var.value, numPlaces);
				}
			} else if (srt::containsObject(objects, noBraces)) {
				srt::Object& obj = srt::findObject(objects, noBraces);
				try {
					newString += obj.getRep(numPlaces, vars, objects, structs, srt::NO_SUPER_REP);
				} catch (const runtime_error& e) { // no suitable string representation
					cout << ANSI_RED << "No suitable string representation found for object \"" << obj.name << "\".\n"
						 << "This could be due to no suitable string representation for one or more of its inner objects.\n"
						 << "If you used superstruct string-rep reference, then the error could also be due to no suitable string "
						 << "representation for one or more of its superstructs (both direct and indirect).\n" << ANSI_NORMAL;
					s = "";
					return;
				}
			} else {
				cout << ANSI_RED << "There is currently no variable or object named \"" << noBraces << "\".\n" << ANSI_NORMAL;
				s = "";
				return;
			}
		}
	}
	s = newString;
}

void var::replaceVariableReferencesWithFullPrecisionValues(string& s, vector<Variable>& vars,
														   vector<srt::Object>& objects, vector<srt::Struct>& structs) {
	vector<string> parts = strUtil::partsSplitByOpenCloseDelimiters(s, '[', ']');
	string newString;
	for (const string& part : parts) {
		if (!strUtil::contains(part, "[")) {
			// this part has no square brackets
			newString += part;
		} else {
			// this part has square brackets
			string noSquareBrackets = part.substr(1, part.size() - 2); // removes opening and closing square brackets
			if (var::contains(vars, noSquareBrackets)) {
				Variable& var = var::find(vars, noSquareBrackets);
				newString += var.value;
			} else if (srt::containsObject(objects, noSquareBrackets)) {
				srt::Object& obj = srt::findObject(objects, noSquareBrackets);
				try {
					newString += obj.getRep(-1, vars, objects, structs, srt::NO_SUPER_REP); // no rounding
				} catch (const runtime_error& e) { // no suitable string representation
					cout << ANSI_RED << "No suitable string representation found for object \"" << obj.name << "\".\n"
						 << "This could be due to no suitable string representation for one or more of its inner objects.\n"
						 << "If you used superstruct string-rep reference, then the error could also be due to no suitable string "
						 << "representation for one or more of its superstructs (both direct and indirect).\n" << ANSI_NORMAL;
					s = "";
					return;
				}
			} else {
				cout << ANSI_RED << "There is currently no variable or object named \"" << noSquareBrackets << "\".\n" << ANSI_NORMAL;
				s = "";
				return;
			}
		}
	}
	s = newString;
}

bool var::isPrimitive(const string& type) {
	return type == "Number" || type == "Bool" || type == "String";
}