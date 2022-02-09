#include "file.h"

// only use for strings!
void file::inputStrFrom(string& v_str, const string& filename) {
    ifstream fin(filename);
    getline(fin, v_str);
    fin.close();
}

// only use for strings!
void file::inputStrVecFrom(vector<string>& v_var, const string& filename) {
    ifstream fin(filename);
    string temp;
    while (!fin.fail()) {
        getline(fin, temp);
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