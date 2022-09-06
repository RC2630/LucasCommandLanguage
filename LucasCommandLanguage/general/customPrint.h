#ifndef CUSTOM_PRINT_DOT_H
#define CUSTOM_PRINT_DOT_H

#include <iostream>
#include <map>
#include <vector>
#include <functional>

using namespace std;

// The purpose of this file is to facilitate the output of custom objects (including library ones).
// It will be useful for any object that doesn't have a built-in operator << for output.
// Because of the nature of templates and requirements for declarations/prototypes in C++,
// the standard utility functions for output in "vectorUtility.h" and "mapUtility.h" are sometimes insufficient.
// And this is exactly why this file exists.

namespace cprint {

	// the purpose of this function is to facilitate the chaining of operator <<
	ostream& operator << (ostream& out, const ostream& ignore);

	// the purpose of this function is to display vectors of custom objects
    template <typename T>
    ostream& print(ostream& out, const vector<T>& v, ostream& (*printFunc) (ostream&, const T&)) {
        if (v.empty()) {
            out << "[]";
            return out;
        }
        out << "[";
        for (int i = 0; i < v.size() - 1; i++) {
            printFunc(out, v.at(i)) << ", ";
        }
        printFunc(out, v.back()) << "]";
        return out;
    }

    // the purpose of this function is to display vectors of custom objects (_f version)
    template <typename T>
    ostream& print_f(ostream& out, const vector<T>& v, function<ostream& (ostream&, const T&)> printFunc) {
        if (v.empty()) {
            out << "[]";
            return out;
        }
        out << "[";
        for (int i = 0; i < v.size() - 1; i++) {
            printFunc(out, v.at(i)) << ", ";
        }
        printFunc(out, v.back()) << "]";
        return out;
    }

	// the purpose of this function is to display maps of custom objects
    template <typename K, typename V>
    ostream& print(ostream& out, const map<K, V>& m,
                   ostream& (*printKey) (ostream&, const K&), ostream& (*printValue) (ostream&, const V&)) {
        if (m.empty()) {
            out << "[]";
            return out;
        }
        out << "[";
        auto iterator = m.begin();
        auto endMinus1 = m.end();
        endMinus1--;
        for (; iterator != endMinus1; iterator++) {
            pair<K, V> p = *iterator;
            printKey(out, p.first) << " : " << printValue(out, p.second) << ", ";
        }
        pair<K, V> last = *endMinus1;
        printKey(out, last.first) << " : " << printValue(out, last.second) << "]";
        return out;
    }

    // the purpose of this function is to display maps of custom objects (_f version)
    template <typename K, typename V>
    ostream& print_f(ostream& out, const map<K, V>& m,
                     function<ostream& (ostream&, const K&)> printKey, function<ostream& (ostream&, const V&)> printValue) {
        if (m.empty()) {
            out << "[]";
            return out;
        }
        out << "[";
        auto iterator = m.begin();
        auto endMinus1 = m.end();
        endMinus1--;
        for (; iterator != endMinus1; iterator++) {
            pair<K, V> p = *iterator;
            printKey(out, p.first) << " : " << printValue(out, p.second) << ", ";
        }
        pair<K, V> last = *endMinus1;
        printKey(out, last.first) << " : " << printValue(out, last.second) << "]";
        return out;
    }

}

#endif