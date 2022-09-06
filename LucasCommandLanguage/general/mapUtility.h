#ifndef MAP_UTIL_DOT_H
#define MAP_UTIL_DOT_H

#include <map>
#include <utility>
#include <iostream>
#include <vector>

using namespace std;

namespace mapUtil {

    template <typename K, typename V>
    ostream& operator << (ostream& out, const map<K, V>& m) {
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
            out << p.first << " : " << p.second << ", ";
        }
        pair<K, V> last = *endMinus1;
        out << last.first << " : " << last.second << "]";
        return out;
    }

    template <typename K, typename V>
    vector<pair<K, V>> vectorify(const map<K, V>& m) {
        vector<pair<K, V>> v;
        for (const pair<K, V>& p : m) {
            v.push_back(p);
        }
        return v;
    }

    template <typename K, typename V>
    map<K, V> makeMap(const vector<K>& vk, const vector<V>& vv) {
        map<K, V> m;
        for (int i = 0; i < vk.size(); i++) {
            m.insert({vk.at(i), vv.at(i)});
        }
        return m;
    }

    template <typename T>
    map<T, T> makeMapBothWays(const vector<T>& v1, const vector<T>& v2) {
        map<T, T> m;
        for (int i = 0; i < v1.size(); i++) {
            m.insert({v1.at(i), v2.at(i)});
            m.insert({v2.at(i), v1.at(i)});
        }
        return m;
    }

    template <typename K, typename V>
    map<K, V> makeMapFromVectorOfPairs(const vector<pair<K, V>>& pairs) {
        map<K, V> m;
        for (const auto& pair : pairs) {
            m.insert(pair);
        }
        return m;
    }

    template <typename K, typename V>
    pair<vector<K>, vector<V>> pairOfVectorify(const map<K, V>& m) {
        vector<K> keys;
        vector<V> values;
        for (const auto& [key, value] : m) {
            keys.push_back(key);
            values.push_back(value);
        }
        return {keys, values};
    }

    template <typename K, typename V>
    vector<K> getKeys(const map<K, V>& m) {
        vector<K> keys;
        for (const auto& [key, value] : m) {
            keys.push_back(key);
        }
        return keys;
    }

    template <typename K, typename V>
    vector<V> getValues(const map<K, V>& m) {
        vector<V> values;
        for (const auto& [key, value] : m) {
            values.push_back(value);
        }
        return values;
    }

    template <typename K, typename V>
    V getOrDefault(const map<K, V>& m, const K& key, const V& defaultValue) {
        if (m.count(key)) {
            return m.at(key);
        } else {
            return defaultValue;
        }
    }

    template <typename T>
    T getOrSelf(const map<T, T>& m, const T& key) {
        if (m.count(key)) {
            return m.at(key);
        } else {
            return key;
        }
    }

}

#endif