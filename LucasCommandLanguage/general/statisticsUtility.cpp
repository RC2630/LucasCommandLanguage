#include "statisticsUtility.h"

double statUtil::mean(const vector<double>& data) {
    return sum(data) / data.size();
}

double statUtil::median(const vector<double>& data) {
    vector<double> copy = data;
    sort(copy.begin(), copy.end());
    if (copy.size() % 2 == 1) {
        return copy.at(copy.size() / 2);
    } else {
        return mean({copy.at(copy.size() / 2), copy.at(copy.size() / 2 - 1)});
    }
}

double statUtil::range(const vector<double>& data) {
    return max(data) - min(data);
}

// popOrSample must be one of: "population", "sample"
double statUtil::variance(const vector<double>& data, const string& popOrSample) {
    int denominator = (popOrSample == "population") ? data.size() : (data.size() - 1);
    double dataMean = mean(data);
    double sumOfSquaredDists = accumulate(data.begin(), data.end(), 0.0, [dataMean](double rsf, double curr) {
        return rsf + pow(curr - dataMean, 2);
        });
    return sumOfSquaredDists / denominator;
}

// popOrSample must be one of: "population", "sample"
double statUtil::stdDev(const vector<double>& data, const string& popOrSample) {
    return sqrt(variance(data, popOrSample));
}

// popOrSample must be one of: "population", "sample"
vector<double> statUtil::scale(const vector<double>& unscaledData, const string& popOrSample) {
    double dataMean = mean(unscaledData);
    double dataSD = stdDev(unscaledData, popOrSample);
    vector<double> scaledData;
    for (double n : unscaledData) {
        scaledData.push_back((n - dataMean) / dataSD);
    }
    return scaledData;
}

// returns true if v1 is earlier in permutation order than v2
// the permutation order is the order in the example below
bool statUtil::earlierInPermutationOrder(const vector<int>& v1, const vector<int>& v2) {
    for (int i = 0; i < v1.size(); i++) {
        if (v1.at(i) < v2.at(i)) {
            return true;
        } else if (v1.at(i) > v2.at(i)) {
            return false;
        }
    }
    throw runtime_error("all elements of v1 and v2 appear to be equal");
}

// generates permutations from 1 to n
// example: generatePermutations(3) = {{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}}
vector<vector<int>> statUtil::generatePermutations(int n) {
    if (n == 0) {
        return {};
    } else if (n == 1) {
        return {{1}};
    } else {
        vector<vector<int>> prev = generatePermutations(n - 1);
        vector<vector<int>> result;
        for (const vector<int>& perm : prev) {
            for (int i = 0; i <= perm.size(); i++) {
                vector<int> temp = perm;
                vecUtil::insertAtPos(temp, i, n);
                int indexToInsert = 0;
                for (int r = 0; r < result.size(); r++) {
                    if (earlierInPermutationOrder(result.at(r), temp)) {
                        indexToInsert++;
                    } else {
                        break;
                    }
                }
                vecUtil::insertAtPos(result, indexToInsert, temp);
            }
        }
        return result;
    }
}