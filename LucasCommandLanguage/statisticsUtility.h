#ifndef STAT_UTIL_DOT_H
#define STAT_UTIL_DOT_H

#include "numberUtility.h"
#include "vectorUtility.h"
#include "abstractFunctions.h"

#include <map>
#include <numeric>
#include <algorithm>

using namespace std;

// for functions that operate on vector<double>, they are more purely data-oriented, and work only on numerical data
// for functions that operate on vector<T>, they are more flexible with the type of data, but require you to define operators
// all functions that operate on vector<T> can work on vector<double> as well, since double has pre-defined operators <, ==, and +

namespace statUtil {

    // operator + must be defined for type T, and for any values t1, t2 of type T, t1 + t2 must produce a sum of type T as well
    // also, if T is a custom data type, such as a struct or class, a default constructor (with 0 arguments) must be publicly accessible
    template <typename T>
    T sum(const vector<T>& data) {
        return accumulate(data.begin(), data.end(), T());
    }

    double mean(const vector<double>& data);
    double median(const vector<double>& data);

    // operator < must be defined for type T, such that the data can be sorted
    template <typename T>
    T max(const vector<T>& data) {
        return *max_element(data.begin(), data.end());
    }

    // operator < must be defined for type T, such that the data can be sorted
    template <typename T>
    T min(const vector<T>& data) {
        return *min_element(data.begin(), data.end());
    }

    double range(const vector<double>& data);
    // popOrSample must be one of: "population", "sample"
    double variance(const vector<double>& data, const string& popOrSample);
    // popOrSample must be one of: "population", "sample"
    double stdDev(const vector<double>& data, const string& popOrSample);
    // popOrSample must be one of: "population", "sample"
    vector<double> scale(const vector<double>& unscaledData, const string& popOrSample);

    // operator == must be defined for type T, so that any values t1, t2 of type T must either compare equal or not equal
    template <typename T>
    map<T, int> frequencyTable(const vector<T>& data) {
        map<T, int> freqMap;
        for (const T& e : data) {
            freqMap[e]++;
        }
        return freqMap;
    }

    // operator == must be defined for type T, so that any values t1, t2 of type T must either compare equal or not equal
    template <typename T>
    T mode(const vector<T>& data) {
        map<T, int> freqMap = frequencyTable(data);
        return max_element(freqMap.begin(), freqMap.end(), [](const pair<T, int>& p1, const pair<T, int>& p2) {
            return p1.second < p2.second;
            })->first;
    }

    // returns true if v1 is earlier in permutation order than v2
    // the permutation order is the order in the example below
    bool earlierInPermutationOrder(const vector<int>& v1, const vector<int>& v2);

    // generates permutations from 1 to n
    // example: generatePermutations(3) = {{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}}
    vector<vector<int>> generatePermutations(int n);

}

#endif