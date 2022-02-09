#ifndef ABSTRACT_FUNC_DOT_H
#define ABSTRACT_FUNC_DOT_H

#include "vectorUtility.h"
#include <functional>

using namespace std;

namespace absFunc {

    // constructs a vector with n copies of e
    template <typename T>
    vector<T> makeList(int n, const T& e) {
        vector<T> v;
        for (int i = 1; i <= n; i++) {
            v.push_back(e);
        }
        return v;
    }

    // constructs a vector consisting of {mapFunc(0), mapFunc(1), ..., mapFunc(n - 1)}
    template <typename T>
    vector<T> buildList(int n, T(*mapFunc) (int)) {
        vector<T> v;
        for (int i = 0; i < n; i++) {
            v.push_back(mapFunc(i));
        }
        return v;
    }

    template <typename T>
    vector<T> buildList_f(int n, function<T(int)> mapFunc) {
        vector<T> v;
        for (int i = 0; i < n; i++) {
            v.push_back(mapFunc(i));
        }
        return v;
    }

    // filters the vector, keeping only elements that satisfy predicate
    template <typename T>
    vector<T> filter(const vector<T>& v, bool (*predicate) (const T&)) {
        vector<T> f;
        for (const T& e : v) {
            if (predicate(e)) {
                f.push_back(e);
            }
        }
        return f;
    }

    template <typename T>
    vector<T> filter_f(const vector<T>& v, function<bool(const T&)> predicate) {
        vector<T> f;
        for (const T& e : v) {
            if (predicate(e)) {
                f.push_back(e);
            }
        }
        return f;
    }

    // maps each element e in v to mapFunc(e) in the new vector
    template <typename O, typename N>
    vector<N> map(const vector<O>& old, N(*mapFunc) (const O&)) {
        vector<N> n;
        for (const O& o : old) {
            n.push_back(mapFunc(o));
        }
        return n;
    }

    template <typename O, typename N>
    vector<N> map_f(const vector<O>& old, function<N(const O&)> mapFunc) {
        vector<N> n;
        for (const O& o : old) {
            n.push_back(mapFunc(o));
        }
        return n;
    }

    // returns true iff for all elements of v, predicate(v) returns true
    template <typename T>
    bool andmap(const vector<T>& v, bool (*predicate) (const T&)) {
        bool result = true;
        for (const T& e : v) {
            result = result && predicate(e);
        }
        return result;
    }

    template <typename T>
    bool andmap_f(const vector<T>& v, function<bool(const T&)> predicate) {
        bool result = true;
        for (const T& e : v) {
            result = result && predicate(e);
        }
        return result;
    }

    // returns true iff there exists (at least) one element of v, such that predicate(v) returns true
    template <typename T>
    bool ormap(const vector<T>& v, bool (*predicate) (const T&)) {
        bool result = false;
        for (const T& e : v) {
            result = result || predicate(e);
        }
        return result;
    }

    template <typename T>
    bool ormap_f(const vector<T>& v, function<bool(const T&)> predicate) {
        bool result = false;
        for (const T& e : v) {
            result = result || predicate(e);
        }
        return result;
    }

    // NOTE: there is no foldr() or foldl() here, because you can use std::accumulate() for essentially the same thing

}

#endif