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

    // inserts e into v at index pos, so that v.at(pos) == e after the insert
    template <typename T>
    void insertAtPos(vector<T>& v, int pos, const T& e) {
        vector<T> newVector;
        for (int i = 0; i < pos; i++) {
            newVector.push_back(v.at(i));
        }
        newVector.push_back(e);
        for (int i = pos; i < v.size(); i++) {
            newVector.push_back(v.at(i));
        }
        v = newVector;
    }

    // splices the elements of spliceInput into v at index pos, so that v.at(pos) == spliceInput.at(0) after the splice
    template <typename T>
    void spliceAtPos(vector<T>& v, int pos, const vector<T>& spliceInput) {
        vector<T> newVector;
        for (int i = 0; i < pos; i++) {
            newVector.push_back(v.at(i));
        }
        for (int i = 0; i < spliceInput.size(); i++) {
            newVector.push_back(spliceInput.at(i));
        }
        for (int i = pos; i < v.size(); i++) {
            newVector.push_back(v.at(i));
        }
        v = newVector;
    }

    // removes the first instance of e from v
    template <typename T>
    void removeFirstInstance(vector<T>& v, const T& e) {
        vector<T> newVector;
        int pos;
        for (pos = 0; pos < v.size(); pos++) {
            if (v.at(pos) == e) {
                // do not append onto newVector
                pos++;
                break;
            } else {
                newVector.push_back(v.at(pos));
            }
        }
        for (int i = pos; i < v.size(); i++) {
            newVector.push_back(v.at(i));
        }
        v = newVector;
    }

    // removes all instances of e from v
    template <typename T>
    void removeAllInstances(vector<T>& v, const T& e) {
        vector<T> newVector;
        for (const T& t : v) {
            if (t != e) {
                newVector.push_back(t);
            }
        }
        v = newVector;
    }

}

#endif