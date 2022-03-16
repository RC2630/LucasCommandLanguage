#include "struct.h"

// this constructor throws a runtime error if a self-reference is detected,
// and throws an invalid argument exception if an invalid type is detected
srt::Struct::Struct(const string& name_, const vector<string>& fieldsAndTypesUnpaired, const vector<Struct>& structs) {
	name = name_;
	for (int i = 0; i < fieldsAndTypesUnpaired.size(); i += 2) {
		string field = fieldsAndTypesUnpaired.at(i);
		string type = fieldsAndTypesUnpaired.at(i + 1);
		if (type == name) {
			throw runtime_error(type); // runtime error indicates self-reference
		}
		if (!var::isPrimitive(type) && !srt::containsStruct(structs, type)) {
			throw invalid_argument(type); // invalid argument indicates invalid type
		}
		fieldsAndTypes.push_back({field, type});
	}
	fieldsAndTypesForEquality = fieldsAndTypes;
}

bool srt::Struct::operator == (const Struct& other) const {
	return name == other.name;
}

bool srt::Struct::operator != (const Struct& other) const {
	return name != other.name;
}

bool srt::Struct::containsStructAsInner(const string& innerSrtname) const {
	for (const pair<string, string>& fieldAndType : fieldsAndTypes) {
		if (fieldAndType.second == innerSrtname) {
			return true;
		}
	}
	return false;
}

vector<string> srt::Struct::getFields() const {
	vector<string> fields;
	for (const auto& [field, type] : fieldsAndTypes) {
		fields.push_back(field);
	}
	return fields;
}

vector<string> srt::Struct::getTypes() const {
	vector<string> types;
	for (const auto& [field, type] : fieldsAndTypes) {
		types.push_back(type);
	}
	return types;
}

void srt::Struct::setEqualityFields(const vector<string>& fields) {
	this->fieldsAndTypesForEquality.clear();
	vector<string> allFields = this->getFields();
	for (const string& field : fields) {
		int index = vecUtil::findIndex(allFields, field);
		this->fieldsAndTypesForEquality.push_back(this->fieldsAndTypes.at(index));
	}
}

// if you are calling this constructor from outside (i.e. non-recursively),
// you MUST provide an int variable set to 0 for the fieldInitValuesIndex parameter
// ALSO, this constructor throws an exception on a non-existent inner struct type, so please catch it
srt::Object::Object(const string& name_, const Struct& type_, const vector<string>& fieldInitValues,
					vector<Variable>& vars, vector<Object>& objects, vector<Struct>& structs, int& fieldInitValuesIndex) {
	name = name_;
	structTypename = type_.name;
	for (const pair<string, string>& fieldAndType : type_.fieldsAndTypes) {
		string fieldname = fieldAndType.first;
		string fieldtype = fieldAndType.second;
		if (var::isPrimitive(fieldtype)) {
			vars.push_back(Variable(name_ + "." + fieldname, fieldInitValues.at(fieldInitValuesIndex), fieldtype));
			fieldnames.push_back(vars.back().name);
			fieldInitValuesIndex++;
		} else {
			// we need to be very careful about what needs to be fed into the recursive call
			string innerName = name_ + "." + fieldname;
			if (!srt::containsStruct(structs, fieldtype)) {
				throw invalid_argument(fieldtype);
			}
			Struct& innerType = srt::findStruct(structs, fieldtype);
			// and now, we trust the natural recursion ...
			objects.push_back(Object(innerName, innerType, fieldInitValues, vars, objects, structs, fieldInitValuesIndex));
			objFieldnames.push_back(objects.back().name);
		}
	}
}

bool srt::Object::operator == (const Object& other) const {
	return name == other.name;
}

bool srt::Object::operator != (const Object& other) const {
	return name != other.name;
}

// throws runtime_error if no suitable string representation is found on this object or one of its inner objects
// the parameter numPlaces is greater or equal to 0 if we should round, and is equal to -1 if we should NOT round
// the parameter superRep is a valid string rep if we should use it, and is equal to srt::NO_SUPER_REP if we should use srt.rep instead
string srt::Object::getRep(int numPlaces, vector<Variable>& vars, vector<Object>& objects,
						   vector<Struct>& structs, const string& superRep) const {
	Struct& srt = findStruct(structs, this->structTypename);
	string useThisRep = (superRep == srt::NO_SUPER_REP) ? srt.rep : superRep;
	if (useThisRep.empty()) {
		throw runtime_error("no suitable string representation found");
	}
	string rep;
	vector<string> parts = strUtil::partsSplitByOpenCloseDelimiters(useThisRep, '<', '>');
	for (const string& part : parts) {
		if (!strUtil::contains(part, "<")) {
			// this part doesn't have angle brackets
			rep += part;
		} else {
			// this part has angle brackets
			string fieldname = part.substr(1, part.size() - 2); // removes angle brackets
			if (fieldname.front() == '@') {
				// this is a reference to the string rep of a superstruct
				string supersrtname = fieldname.substr(1);
				Struct& superstruct = findStruct(structs, supersrtname);
				rep += this->getRep(numPlaces, vars, objects, structs, superstruct.rep);
				continue;
			}
			fieldname = this->name + "." + fieldname; // "personalize" the fieldname for the current object
			if (vecUtil::contains(this->fieldnames, fieldname)) {
				// this is a variable field
				Variable& field = var::find(vars, fieldname);
				if (numPlaces == -1) { // full precision
					rep += field.value;
				} else { // should round
					if (field.datatype == "String" || field.datatype == "Bool") {
						rep += field.value;
					} else if (field.datatype == "Number") {
						rep += numUtil::roundToNplaces(field.value, numPlaces);
					}
				}
			} else if (vecUtil::contains(this->objFieldnames, fieldname)) {
				// this is an inner object field
				rep += findObject(objects, fieldname).getRep(numPlaces, vars, objects, structs, srt::NO_SUPER_REP);
			}
		}
	}
	return rep;
}

// compares (recursively) to see if each field is equal, as opposed to simply a name check as in the case of operator ==
// the parameter numPlaces is greater or equal to 0 if we should round, and is equal to -1 if we should NOT round
bool srt::Object::deepEquals(const Object& other, vector<Variable>& vars, vector<Object>& objects,
							 vector<Struct>& structs, int numPlaces) const {
	if (this->name == other.name) {
		return true; // they are literally the same object
	}
	// at this point, they are different objects (but they could be either equal or unequal)
	for (const auto& [fieldname, type] : findStruct(structs, this->structTypename).fieldsAndTypesForEquality) {
		if (var::isPrimitive(type)) {
			// variable field
			Variable& var1 = var::find(vars, this->name + "." + fieldname);
			Variable& var2 = var::find(vars, other.name + "." + fieldname);
			if (!var1.equals(var2, numPlaces)) {
				return false;
			}
		} else {
			// inner object field
			Object& obj1 = srt::findObject(objects, this->name + "." + fieldname);
			Object& obj2 = srt::findObject(objects, other.name + "." + fieldname);
			if (!obj1.deepEquals(obj2, vars, objects, structs, numPlaces)) {
				return false;
			}
		}
	}
	// at this point, they must be equal objects
	return true;
}

// inserts values of all fields (including nested inner ones) into the end of fieldInitValues
void srt::Object::copyFieldsTo(vector<string>& fieldInitValues, vector<Variable>& vars,
							   vector<Object>& objects, vector<Struct>& structs) const {
	Struct& srt = findStruct(structs, this->structTypename);
	for (const auto& [fieldname, type] : srt.fieldsAndTypes) {
		if (isPrimitive(type)) {
			// variable field
			Variable& field = var::find(vars, this->name + "." + fieldname);
			fieldInitValues.push_back(field.value);
		} else {
			// inner object
			Object& inner = findObject(objects, this->name + "." + fieldname);
			inner.copyFieldsTo(fieldInitValues, vars, objects, structs);
		}
	}
}

bool srt::containsStruct(const vector<Struct>& structs, const string& srtname) {
	for (const Struct& srt : structs) {
		if (srt.name == srtname) {
			return true;
		}
	}
	return false;
}
bool srt::containsObject(const vector<Object>& objects, const string& objname) {
	for (const Object& obj : objects) {
		if (obj.name == objname) {
			return true;
		}
	}
	return false;
}

srt::Struct& srt::findStruct(vector<Struct>& structs, const string& srtname) {
	for (Struct& srt : structs) {
		if (srt.name == srtname) {
			return srt;
		}
	}
	throw runtime_error("no matching struct");
}

srt::Object& srt::findObject(vector<Object>& objects, const string& objname) {
	for (Object& obj : objects) {
		if (obj.name == objname) {
			return obj;
		}
	}
	throw runtime_error("no matching object");
}

bool srt::containsStructAsInner(const vector<Struct>& structs, const string& innerSrtname) {
	for (const Struct& srt : structs) {
		if (srt.containsStructAsInner(innerSrtname)) {
			return true;
		}
	}
	return false;
}

bool srt::isSuperstruct(const vector<Struct>& structs, const string& srtname) {
	for (const Struct& srt : structs) {
		if (vecUtil::contains(srt.superstructs, srtname)) {
			return true;
		}
	}
	return false;
}

void srt::deleteObject(vector<Object>& objects, vector<Variable>& vars, const string& objname) {
	if (!containsObject(objects, objname)) {
		return;
	}
	Object obj = findObject(objects, objname);
	vector<int> indexesToRemove;
	for (int i = 0; i < vars.size(); i++) {
		if (strUtil::beginsWith(vars.at(i).name, objname + ".")) {
			indexesToRemove.push_back(i);
		}
	}
	vecUtil::removeByIndexes(vars, indexesToRemove);
	for (const string& innerObjname : obj.objFieldnames) {
		// recursively delete inner objects
		deleteObject(objects, vars, innerObjname);
	}
	vecUtil::removeFirstInstance(objects, obj);
}

// no recursive inner struct deletion is necessary,
// since an outer struct's deletion does NOT logically result in its inner structs' deletion as well
// (unlike outer/inner objects, for which recursive deletion DOES make sense)
void srt::deleteStruct(vector<Struct>& structs, vector<Object>& objects, vector<Variable>& vars, const string& srtname) {
	if (!containsStruct(structs, srtname)) {
		return;
	}
	if (containsStructAsInner(structs, srtname)) {
		cout << ANSI_RED << "The struct you are attempting to delete (" << srtname << ") is an inner struct of at least 1 other struct. Therefore, the deletion cannot proceed.\n" << ANSI_NORMAL;
		return;
	}
	if (isSuperstruct(structs, srtname)) {
		cout << ANSI_RED << "The struct you are attempting to delete (" << srtname << ") is a superstruct of at least 1 other struct. Therefore, the deletion cannot proceed.\n" << ANSI_NORMAL;
		return;
	}
	Struct srt = findStruct(structs, srtname);
	vector<string> namesToDelete;
	for (const Object& obj : objects) {
		if (obj.structTypename == srtname) {
			namesToDelete.push_back(string(obj.name));
		}
	}
	for (const string& nameToDelete : namesToDelete) {
		deleteObject(objects, vars, nameToDelete);
	}
	vecUtil::removeFirstInstance(structs, srt);
}

void srt::attemptStructDefinition(const string& name, const vector<string>& fieldAndTypesUnpaired,
								  vector<Struct>& structs, bool& nonAssertCrash) {
	try {
		structs.push_back(Struct(name, fieldAndTypesUnpaired, structs));
	} catch (const invalid_argument& e) {
		cout << ANSI_RED << "An invalid type (" << e.what() << ") has been detected. Program will now terminate.\n" << ANSI_NORMAL;
		nonAssertCrash = false; // this special error is treated like an assertion
		throw runtime_error("invalid type"); // do not catch this (except in main()'s catch-all)
	} catch (const runtime_error& e) {
		cout << ANSI_RED << "A self-reference on a struct (" << e.what() << ") has been detected. Program will now terminate.\n" << ANSI_NORMAL;
		nonAssertCrash = false; // this special error is treated like an assertion
		throw runtime_error("self-reference"); // do not catch this (except in main()'s catch-all)
	}
}

void srt::attemptObjectConstruction(const string& name, const Struct& type, const vector<string>& fieldInitValues,
									vector<Variable>& vars, vector<Object>& objects, vector<Struct>& structs, bool& nonAssertCrash) {
	try {
		int i = 0;
		objects.push_back(Object(name, type, fieldInitValues, vars, objects, structs, i));
	} catch (const invalid_argument& e) {
		cout << ANSI_RED << "A non-existent inner struct (" << e.what() << ") has been detected. Program will now terminate.\n" << ANSI_NORMAL;
		nonAssertCrash = false; // this special error is treated like an assertion
		throw runtime_error("non-existent inner struct"); // do not catch this (except in main()'s catch-all)
	}
}