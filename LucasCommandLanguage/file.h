#ifndef FILE_DOT_H
#define FILE_DOT_H

#include <fstream>
#include <vector>
#include <string>

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

}

#endif