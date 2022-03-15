#ifndef STRUCT_DOT_H
#define STRUCT_DOT_H

#include "variable.h"

#include <stdexcept>
#include <utility>

namespace var {
	struct Variable; // definition found in "variable.h"
}

using namespace var;

namespace srt {

	struct Struct {

		string name;
		vector<pair<string, string>> fieldsAndTypes;
		vector<pair<string, string>> fieldsAndTypesForEquality;
		vector<string> defaultValues;
		string rep;

		Struct() = default;
		// this constructor throws a runtime error if a self-reference is detected,
		// and throws an invalid argument exception if an invalid type is detected
		Struct(const string& name_, const vector<string>& fieldsAndTypesUnpaired, const vector<Struct>& structs);

		bool operator == (const Struct& other) const;
		bool operator != (const Struct& other) const;
		bool containsStructAsInner(const string& innerSrtname) const;
		vector<string> getFields() const;
		vector<string> getTypes() const;
		void setEqualityFields(const vector<string>& fields);

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

		// throws runtime_error if no suitable string representation is found on this object or one of its inner objects
		// the parameter numPlaces is greater or equal to 0 if we should round, and is equal to -1 if we should NOT round
		string getRep(int numPlaces, vector<Variable>& vars, vector<Object>& objects, vector<Struct>& structs) const;

		// compares (recursively) to see if each field is equal, as opposed to simply a name check as in the case of operator ==
		// the parameter numPlaces is greater or equal to 0 if we should round, and is equal to -1 if we should NOT round
		bool deepEquals(const Object& other, vector<Variable>& vars, vector<Object>& objects, vector<Struct>& structs, int numPlaces) const;

		// inserts values of all fields (including nested inner ones) into the end of fieldInitValues
		void copyFieldsTo(vector<string>& fieldInitValues, vector<Variable>& vars, vector<Object>& objects, vector<Struct>& structs) const;

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