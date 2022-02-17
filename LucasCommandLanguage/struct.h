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
		// this constructor throws a runtime error if a self-reference is detected,
		// and throws an invalid argument exception if an invalid type is detected
		Struct(const string& name_, const vector<string>& fieldsAndTypesUnpaired, const vector<Struct>& structs);

		bool operator == (const Struct& other) const;
		bool operator != (const Struct& other) const;
		bool containsStructAsInner(const string& innerSrtname) const;

	};

	struct Object {

		string name;
		string structTypename;
		vector<string> fieldnames; // encompasses only variables
		vector<string> objFieldnames; // encompasses nested inner object fields

		// if you are calling this constructor from outside (i.e. non-recursively),
		// you MUST provide an int variable set to 0 for the fieldInitValuesIndex parameter
		// ALSO, this constructor throws an exception on a non-existent inner struct type, so please catch it
		Object(const string& name_, const Struct& type_, const vector<string>& fieldInitValues,
			   vector<Variable>& vars, vector<Object>& objects, vector<Struct>& structs, int& fieldInitValuesIndex);

		bool operator == (const Object& other) const;
		bool operator != (const Object& other) const;

	};

	bool containsStruct(const vector<Struct>& structs, const string& srtname);
	bool containsObject(const vector<Object>& objects, const string& objname);
	Struct& findStruct(vector<Struct>& structs, const string& srtname);
	Object& findObject(vector<Object>& objects, const string& objname);
	bool containsStructAsInner(const vector<Struct>& structs, const string& innerSrtname);
	void deleteObject(vector<Object>& objects, vector<Variable>& vars, const string& objname);
	void deleteStruct(vector<Struct>& structs, vector<Object>& objects, vector<Variable>& vars, const string& srtname);
	void attemptStructDefinition(const string& name, const vector<string>& fieldAndTypesUnpaired,
								 vector<Struct>& structs, bool& nonAssertCrash);
	void attemptObjectConstruction(const string& name, const Struct& type, const vector<string>& fieldInitValues,
								   vector<Variable>& vars, vector<Object>& objects, vector<Struct>& structs, bool& nonAssertCrash);

}

#endif