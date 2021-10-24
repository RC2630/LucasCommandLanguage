#ifndef VECTOR_UTIL_DOT_H
#define VECTOR_UTIL_DOT_H

#include <vector>
#include <iostream>

using namespace std;

namespace vecUtil {

    template <typename T>
    ostream& operator << (ostream& out, const vector<T>& v) {
        if (v.empty()) {
            out << "[]";
            return out;
        }
        out << "[";
        for (int i = 0; i < v.size() - 1; i++) {
            out << v.at(i) << ", ";
        }
        out << v.back() << "]";
        return out;
    }

    template <typename T>
    bool contains(const vector<T>& v, const T& t) {
        for (const T& e : v) {
            if (e == t) {
                return true;
            }
        }
        return false;
    }

}

#endif