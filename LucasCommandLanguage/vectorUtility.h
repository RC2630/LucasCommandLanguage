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

    template <typename T>
    bool strictlyIncreasing(const vector<T>& v) {
        if (v.size() <= 1) {
            return true;
        }
        bool result = true;
        for (int i = 0; i < v.size() - 1; i++) {
            result = result && (v.at(i) < v.at(i + 1));
        }
        return result;
    }

    template <typename T>
    bool strictlyDecreasing(const vector<T>& v) {
        if (v.size() <= 1) {
            return true;
        }
        bool result = true;
        for (int i = 0; i < v.size() - 1; i++) {
            result = result && (v.at(i) > v.at(i + 1));
        }
        return result;
    }

    template <typename T>
    bool generallyIncreasing(const vector<T>& v) {
        if (v.size() <= 1) {
            return true;
        }
        bool result = true;
        for (int i = 0; i < v.size() - 1; i++) {
            result = result && (v.at(i) <= v.at(i + 1));
        }
        return result;
    }

    template <typename T>
    bool generallyDecreasing(const vector<T>& v) {
        if (v.size() <= 1) {
            return true;
        }
        bool result = true;
        for (int i = 0; i < v.size() - 1; i++) {
            result = result && (v.at(i) >= v.at(i + 1));
        }
        return result;
    }

    template <typename T>
    bool allEqual(const vector<T>& v) {
        if (v.size() <= 1) {
            return true;
        }
        bool result = true;
        for (int i = 0; i < v.size() - 1; i++) {
            result = result && (v.at(i) == v.at(i + 1));
        }
        return result;
    }

    template <typename T>
    bool atLeast1NotEqual(const vector<T>& v) {
        return !allEqual(v);
    }

}

#endif