#include "file.h"

// only use for strings!
void file::inputStrFrom(string& v_str, const string& filename) {
    ifstream fin(filename);
    getline(fin, v_str); // no ws (not necessary when only reading 1 line)
    fin.close();
}

// only use for strings!
void file::inputStrVecFrom(vector<string>& v_var, const string& filename) {
    ifstream fin(filename);
    string temp;
    while (!fin.fail()) {
        getline(fin, temp); // no ws (otherwise blank lines will get skipped)
        v_var.push_back(temp);
    }
    v_var.pop_back();
    fin.close();
}

// original type in the file must be string
// only use for strings!
void file::outputStrAddTo(const string& str, const string& filename) {
    vector<string> tempv;
    inputStrVecFrom(tempv, filename);
    tempv.push_back(str);
    outputVecTo(tempv, filename);
}

// original type in the file must be string
// only use for strings!
void file::outputStrVecAddTo(const vector<string>& v, const string& filename) {
    vector<string> tempv;
    inputStrVecFrom(tempv, filename);
    for (const string& s : v) {
        tempv.push_back(s);
    }
    outputVecTo(tempv, filename);
}

void file::clearFile(const string& filename) {
    ofstream fout(filename);
    fout << "";
    fout.close();
}

bool file::isEmpty(const string& filename) {
    vector<string> fileContents;
    file::inputStrVecFrom(fileContents, filename);
    return fileContents.empty();
}

// use this if K == string, V == string
void file::inputMapFrom_stringKeyValue(map<string, string>& m, const string& keysFilename, const string& valuesFilename) {
    vector<string> keys;
    vector<string> values;
    inputStrVecFrom(keys, keysFilename);
    inputStrVecFrom(values, valuesFilename);
    if (keys.size() != values.size()) {
        throw runtime_error("keys and values have different sizes");
    }
    for (int i = 0; i < keys.size(); i++) {
        m.insert({keys.at(i), values.at(i)});
    }
}

// use this if K == string, V == string
void file::outputMapAddTo_stringKeyValue(const map<string, string>& m, const string& keysFilename, const string& valuesFilename) {
    map<string, string> tempm;
    inputMapFrom_stringKeyValue(tempm, keysFilename, valuesFilename);
    for (const auto& pair : m) {
        tempm.insert(pair);
    }
    outputMapTo(tempm, keysFilename, valuesFilename);
}