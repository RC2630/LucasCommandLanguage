#ifndef FILE_DOT_H
#define FILE_DOT_H

#include <fstream>
#include <vector>
#include <string>

#include "mapUtility.h"
#include "parseArguments.h"

using namespace std;

namespace file {

    template <typename T>
    void outputTo(const T& value, const string& filename) {
        ofstream fout(filename);
        fout << value;
        fout.close();
    }

    // do not use for strings!
    template <typename T>
    void inputFrom(T& variable, const string& filename) {
        ifstream fin(filename);
        fin >> variable;
        fin.close();
    }

    // only use for strings!
    void inputStrFrom(string& v_str, const string& filename);

    template <typename T>
    void outputVecTo(const vector<T>& v, const string& filename) {
        ofstream fout(filename);
        for (const T& e : v) {
            fout << e << "\n";
        }
        fout.close();
    }

    // do not use for strings!
    template <typename T>
    void inputVecFrom(vector<T>& v_var, const string& filename) {
        ifstream fin(filename);
        T temp;
        while (!fin.fail()) {
            fin >> temp;
            v_var.push_back(temp);
        }
        v_var.pop_back();
        fin.close();
    }

    // only use for strings!
    void inputStrVecFrom(vector<string>& v_var, const string& filename);

    // type must match what is already in the file
    // do not use for strings!
    template <typename T>
    void outputAddTo(const T& value, const string& filename) {
        vector<T> tempv;
        inputVecFrom(tempv, filename);
        tempv.push_back(value);
        outputVecTo(tempv, filename);
    }

    // original type in the file must be string
    // only use for strings!
    void outputStrAddTo(const string& str, const string& filename);

    // type must match what is already in the file
    // do not use for strings!
    template <typename T>
    void outputVecAddTo(const vector<T>& v, const string& filename) {
        vector<T> tempv;
        inputVecFrom(tempv, filename);
        for (const T& e : v) {
            tempv.push_back(e);
        }
        outputVecTo(tempv, filename);
    }

    // original type in the file must be string
    // only use for strings!
    void outputStrVecAddTo(const vector<string>& v, const string& filename);

    void clearFile(const string& filename);
    bool isEmpty(const string& filename);

    template <typename K, typename V>
    void outputMapTo(const map<K, V>& m, const string& keysFilename, const string& valuesFilename) {
        auto [keys, values] = mapUtil::pairOfVectorify(m);
        outputVecTo(keys, keysFilename);
        outputVecTo(values, valuesFilename);
    }

    // use this if K != string, V != string
    template <typename K, typename V>
    void inputMapFrom(map<K, V>& m, const string& keysFilename, const string& valuesFilename) {
        vector<K> keys;
        vector<V> values;
        inputVecFrom(keys, keysFilename);
        inputVecFrom(values, valuesFilename);
        if (keys.size() != values.size()) {
            throw runtime_error("keys and values have different sizes");
        }
        for (int i = 0; i < keys.size(); i++) {
            m.insert({keys.at(i), values.at(i)});
        }
    }

    // use this if K == string, V != string
    template <typename V>
    void inputMapFrom_stringKey(map<string, V>& m, const string& keysFilename, const string& valuesFilename) {
        vector<string> keys;
        vector<V> values;
        inputStrVecFrom(keys, keysFilename);
        inputVecFrom(values, valuesFilename);
        if (keys.size() != values.size()) {
            throw runtime_error("keys and values have different sizes");
        }
        for (int i = 0; i < keys.size(); i++) {
            m.insert({keys.at(i), values.at(i)});
        }
    }

    // use this if K != string, V == string
    template <typename K>
    void inputMapFrom_stringValue(map<K, string>& m, const string& keysFilename, const string& valuesFilename) {
        vector<K> keys;
        vector<string> values;
        inputVecFrom(keys, keysFilename);
        inputStrVecFrom(values, valuesFilename);
        if (keys.size() != values.size()) {
            throw runtime_error("keys and values have different sizes");
        }
        for (int i = 0; i < keys.size(); i++) {
            m.insert({keys.at(i), values.at(i)});
        }
    }

    // use this if K == string, V == string
    void inputMapFrom_stringKeyValue(map<string, string>& m, const string& keysFilename, const string& valuesFilename);

    // use this if K != string, V != string
    template <typename K, typename V>
    void outputMapAddTo(const map<K, V>& m, const string& keysFilename, const string& valuesFilename) {
        map<K, V> tempm;
        inputMapFrom(tempm, keysFilename, valuesFilename);
        for (const auto& pair : m) {
            tempm.insert(pair);
        }
        outputMapTo(tempm, keysFilename, valuesFilename);
    }

    // use this if K == string, V != string
    template <typename V>
    void outputMapAddTo_stringKey(const map<string, V>& m, const string& keysFilename, const string& valuesFilename) {
        map<string, V> tempm;
        inputMapFrom_stringKey(tempm, keysFilename, valuesFilename);
        for (const auto& pair : m) {
            tempm.insert(pair);
        }
        outputMapTo(tempm, keysFilename, valuesFilename);
    }

    // use this if K != string, V == string
    template <typename K>
    void outputMapAddTo_stringValue(const map<K, string>& m, const string& keysFilename, const string& valuesFilename) {
        map<K, string> tempm;
        inputMapFrom_stringValue(tempm, keysFilename, valuesFilename);
        for (const auto& pair : m) {
            tempm.insert(pair);
        }
        outputMapTo(tempm, keysFilename, valuesFilename);
    }

    // use this if K == string, V == string
    void outputMapAddTo_stringKeyValue(const map<string, string>& m, const string& keysFilename, const string& valuesFilename);

    // requires types K and V be convertible (castable) to string
    template <typename K, typename V>
    void outputMapTo_singleFile(const map<K, V>& m, const string& filename, char sep) {
        vector<string> mapContents;
        for (auto [key, value] : m) {
            mapContents.push_back((string) key + sep + (string) value);
        }
        outputVecTo(mapContents, filename);
    }

    // requires types K and V be convertible (castable) from string
    template <typename K, typename V>
    void inputMapFrom_singleFile(map<K, V>& m, const string& filename, char sep) {
        vector<string> mapContents;
        inputStrVecFrom(mapContents, filename);
        for (const string& entry : mapContents) {
            vector<string> keyAndValue = parse::parseAllArguments(entry, true, sep);
            m.insert({(K) keyAndValue.front(), (V) keyAndValue.back()});
        }
    }

    // requires types K and V be convertible (castable) to string
    template <typename K, typename V>
    void outputMapAddTo_singleFile(const map<K, V>& m, const string& filename, char sep) {
        vector<string> mapContents;
        inputStrVecFrom(mapContents, filename);
        for (auto [key, value] : m) {
            mapContents.push_back((string) key + sep + (string) value);
        }
        outputVecTo(mapContents, filename);
    }

}

#endif