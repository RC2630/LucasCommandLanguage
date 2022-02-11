#ifndef STRUCT_DOT_H
#define STRUCT_DOT_H

#include "variable.h"

#include <stdexcept>
#include <utility>

using namespace var;

namespace srt {

	struct Struct {

		string name;
		vector<pair<string, string>> fieldsAndTypes;
		vector<string> defaultValues;

		Struct() = default;
		Struct(const string& name_, const vector<string>& fieldsAndTypesUnpaired);
		bool operator == (const Struct& other) const;
		bool operator != (const Struct& other) const;

	};

	struct Object {

		string name;
		string structTypename;
		vector<string> fieldnames;

		Object(const string& name_, const Struct& type_, const vector<string>& fieldInitValues, vector<Variable>& vars);
		bool operator == (const Object& other) const;
		bool operator != (const Object& other) const;

	};

	bool containsStruct(const vector<Struct>& structs, const string& srtname);
	bool containsObject(const vector<Object>& objects, const string& objname);
	Struct& findStruct(vector<Struct>& structs, const string& srtname);
	Object& findObject(vector<Object>& objects, const string& objname);
	void deleteObject(vector<Object>& objects, vector<Variable>& vars, const string& objname);
	void deleteStruct(vector<Struct>& structs, vector<Object>& objects, vector<Variable>& vars, const string& srtname);

}

#endif