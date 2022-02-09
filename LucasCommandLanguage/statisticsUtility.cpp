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