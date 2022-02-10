#include "struct.h"

srt::Struct::Struct(const string& name_, const vector<string>& fieldsAndTypesUnpaired) {
	name = name_;
	for (int i = 0; i < fieldsAndTypesUnpaired.size(); i += 2) {
		fieldsAndTypes.push_back({fieldsAndTypesUnpaired.at(i), fieldsAndTypesUnpaired.at(i + 1)});
	}
}

bool srt::Struct::operator == (const Struct& other) const {
	return name == other.name;
}

bool srt::Struct::operator != (const Struct& other) const {
	return name != other.name;
}

srt::Object::Object(const string& name_, const Struct& type_, const vector<string>& fieldInitValues, vector<Variable>& vars) {
	name = name_;
	type = type_;
	for (int i = 0; i < fieldInitValues.size(); i++) {
		vars.push_back(Variable(name_ + "." + type_.fieldsAndTypes.at(i).first, fieldInitValues.at(i), type_.fieldsAndTypes.at(i).second));
		fields.push_back(&vars.back());
	}
}

string srt::Object::typeName() const {
	return type.name;
}

bool srt::Object::operator == (const Object& other) const {
	return name == other.name;
}

bool srt::Object::operator != (const Object& other) const {
	return name != other.name;
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
	vecUtil::removeFirstInstance(objects, obj);
}

void srt::deleteStruct(vector<Struct>& structs, vector<Object>& objects, vector<Variable>& vars, const string& srtname) {
	if (!containsStruct(structs, srtname)) {
		return;
	}
	Struct srt = findStruct(structs, srtname);
	vector<string> namesToDelete;
	for (const Object& obj : objects) {
		if (obj.typeName() == srtname) {
			namesToDelete.push_back(string(obj.name));
		}
	}
	for (const string& nameToDelete : namesToDelete) {
		deleteObject(objects, vars, nameToDelete);
	}
	vecUtil::removeFirstInstance(structs, srt);
}