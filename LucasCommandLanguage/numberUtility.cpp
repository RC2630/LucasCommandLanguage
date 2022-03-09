#include "numberUtility.h"

// ROEPCF
double numUtil::round(double value, int numPlaces) {
    double places = pow(10.0, numPlaces);
    return std::round(value * places) / places;
}

// ROEPCF
// does not work correctly for big numbers (i.e. absolute value of d is on the order of 10^10 or bigger)
// works correctly for small numbers though (i.e. absolute value of d is on the order of 10^-10 or smaller)
bool numUtil::doubleIsAnInteger(double d) {
    int i = static_cast<int>(d);
    double diff = d - i;
    return diff == 0;
}

// ROEPCF
// rounds to the nearest integer, but if there are 2 integers equal distance away from value, then return the bigger one
// this means 1.5 -> 2 (because 2 is bigger than 1), and -1.5 -> -1 (because -1 is bigger than -2)
int numUtil::roundToNearestInt(double value) {
    if (doubleIsAnInteger(value)) {
        return static_cast<int>(value);
    }
    double floor = std::floor(value);
    double ceil = std::ceil(value);
    if (abs(floor - value) < abs(ceil - value)) {
        return static_cast<int>(floor);
    } else {
        return static_cast<int>(ceil);
    }
}

// ROEPCF
// requires d to be strictly positive
// requires base >= 2
// this function returns true if base^k = d for some integer k
// note: when d < base and d != 1, this function returns true if base^-n = d, for some positive integer n
//       and when d = 1, this function always returns true because base^0 = 1, and 0 is an integer
// example: isExactPowerOf(n, 2) will return true for any n in the set {..., 16, 8, 4, 2, 1, 0.5, 0.25, 0.125, 0.0625, ...}
//          because there exists an integer i such that 2^i = n for all n in the above set
//          (..., 2^4 = 16, 2^3 = 8, 2^2 = 4, 2^1 = 2, 2^0 = 1, 2^-1 = 0.5, 2^-2 = 0.25, 2^-3 = 0.125, 2^-4 = 0.0625, ...)
bool numUtil::isExactPowerOf(double d, int base) {
    double log = std::log(d) / std::log(base);
    log = round(log, 14); // because internal representation of doubles SUCK
    return doubleIsAnInteger(log);
}

// ROEPCF
// requires d to be non-negative
int numUtil::numCharBeforeDot(double d) {
    // 0 <= d < 10
    if (d < 10) {
        return 1;
    }
    // 10 <= d <= DBL_MAX
    int extra = 0;
    if (isExactPowerOf(d, 10)) { // because the built in log is kind of stupid and has off by 1 issues
        extra = 1;
    }
    return ceil(log(d, 10)) + extra;
}

// ROEPCF
// warning: if abs(n) is particularly big, or n has particularly many decimal places of precision, the result of this function can suffer
//          in precision (< 14 decimal digits of precision). However, this is the same as what happens if you do this in any other
//          programming language, because this is just another effect of hardware limitations
// gets the i-th decimal place digit of n and return it as a character
char numUtil::digitOf(double n, int i) {
    n = abs(n); // since the i-th decimal digit of n is always the same as the i-th decimal digit of -n, for all i and all n
    double iv = pow(10, i - 1) * n; // intermediate value
    int digit = static_cast<int>(10 * round(iv - floor(iv), 14 - i));
    return static_cast<char>(digit + 48);

    // THE CODE BELOW IS A MORE VERBOSE VERSION OF THIS FUNCTION, USED FOR DEBUGGING PURPOSES:
    //n = abs(n);
    //double iv1__pow_10_i_minus_1 = pow(10, i - 1);
    //double iv2__iv1_times_n = iv1__pow_10_i_minus_1 * n;
    //double iv3__floor_iv2 = floor(iv2__iv1_times_n);
    //double iv4__iv2_minus_iv3 = iv2__iv1_times_n - iv3__floor_iv2;
    //double iv5__round_iv4_14_minus_i = round(iv4__iv2_minus_iv3, 14 - i);
    //double iv6__10_times_iv5 = 10 * iv5__round_iv4_14_minus_i;
    //int iv7__floor_iv6 = static_cast<int>(iv6__10_times_iv5);
    //int digit = iv7__floor_iv6;
    //return static_cast<char>(digit + 48);
}

// ROEPCF
// warning: see warning for digitOf(), since this function uses digitOf()
// converts given double to its string representation with 14 decimal places
string numUtil::doubleToString(double d) {
    double n = round(d, 14); // round to 14 decimal places BEFORE the conversion starts
    string rep;
    if (-1 < n && n < 0) { // for -1 < n < 0, the default to_string() gives 0, but here we actually want -0
        rep += "-";
    }
    rep += to_string(static_cast<int>(n)); // using the default to_string() for the part of the representation before the decimal point
    rep += "."; // appending the decimal point
    for (int i = 1; i <= 14; i++) {
        rep += digitOf(n, i);
    }
    strUtil::replaceAllChar1WithChar2(rep, ':', '0');
    return rep;
}

// ROEPCF
// warning: see warning for digitOf(), since this function uses doubleToString(), which in turn uses digitOf()
string numUtil::toStringRemoveTrailingZeros(double num) {
    string rawToString = doubleToString(num);
    int indexLastNot0 = -1; // should never remain -1 because every double's string representation has 1 decimal point ('.')
    for (int i = rawToString.size() - 1; i >= 0; i--) {
        if (rawToString.at(i) != '0') {
            indexLastNot0 = i;
            break;
        }
    }
    string goodToString;
    for (int i = 0; i <= indexLastNot0; i++) {
        goodToString += rawToString.at(i);
    }
    if (goodToString.at(goodToString.size() - 1) == '.') { // remove decimal point if it is the last character
        goodToString = strUtil::removeChars(goodToString, '.', 1);
    }
    return goodToString;
}

// EVERYTHING AFTER THIS POINT IS AUTOMATICALLY ROEPCF, UNLESS OTHERWISE SPECIFIED

double numUtil::log(double arg, double base) {
    return std::log(arg) / std::log(base);
}

double numUtil::radToDeg(double rad) {
    return rad / PI * 180;
}

double numUtil::degToRad(double deg) {
    return deg / 180 * PI;
}

double numUtil::degreeMode(double (*trigFuncRad) (double), double deg) {
    return trigFuncRad(degToRad(deg));
}

double numUtil::degreeMode_f(function<double(double)> trigFuncRad, double deg) {
    return trigFuncRad(degToRad(deg));
}

int numUtil::factorial(int n) {
    if (n < 0) {
        throw invalid_argument("factorial cannot be called on negative integers");
    }
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int numUtil::numPermutations(int total, int choose) {
    return factorial(total) / factorial(total - choose);
}

int numUtil::numCombinations(int total, int choose) {
    return factorial(total) / (factorial(total - choose) * factorial(choose));
}

int numUtil::multinomialCoefficient(int n, const vector<int>& ks) {
    int sumKs = accumulate(ks.begin(), ks.end(), 0);
    if (sumKs != n) {
        throw invalid_argument("sum of all the k's (" + to_string(sumKs) + ") is not equal to n (" + to_string(n) + ")");
    }
    vector<int> kFacts = absFunc::map<int, int>(ks, [](const int& n) { return factorial(n); });
    int prodKfacts = accumulate(kFacts.begin(), kFacts.end(), 1, [](int a, int b) { return a * b; });
    return factorial(n) / prodKfacts;
}

int numUtil::numDecimalPlaces(const string& num) {
    string simplified = simplify(num);
    if (!strUtil::contains(simplified, ".")) {
        return 0;
    } else {
        string after = strUtil::removeAllBeforeChar(simplified, '.');
        return after.size() - 1;
    }
}

string numUtil::roundToNplaces(const string& num, int n) {
    string simplified = simplify(num);
    if (simplified.at(0) == '-') {
        string rounded = roundToNplaces(simplified.substr(1), n);
        return (rounded == "0") ? "0" : ("-" + rounded);
    }
    if (numDecimalPlaces(simplified) <= n) {
        return simplified;
    }
    string extraDigits = simplified.substr(simplified.size() - (numDecimalPlaces(simplified) - n));
    string keptPart = simplified.substr(0, simplified.size() - (numDecimalPlaces(simplified) - n));
    if (extraDigits.at(0) >= '5') {
        // round up
        int currIndex = keptPart.size() - 1;
        while (currIndex >= 0) {
            if (keptPart.at(currIndex) == '.') {
                // ignore the dot
                currIndex--;
                continue;
            }
            int currDigit = keptPart.at(currIndex) - 48; // ASCII conversion
            if (currDigit + 1 <= 9) {
                keptPart.at(currIndex)++;
                return simplify(keptPart);
            } else {
                keptPart.at(currIndex) = '0';
                currIndex--;
            }
        }
        // by this point, all digits are exhausted and set to 0 (currIndex == -1)
        keptPart = '1' + keptPart;
        return simplify(keptPart);
    } else {
        // round down
        return simplify(keptPart);
    }
}

bool numUtil::hasTrailingZeros(const string& num) {
    if (!strUtil::contains(num, ".")) {
        return false;
    }
    return num.back() == '0';
}

string numUtil::removeTrailingZeros(const string& num) {
    if (!hasTrailingZeros(num)) {
        return num;
    } else {
        return strUtil::removeTrailingCharacters(num, '0');
    }
}

bool numUtil::hasLeadingZeros(const string& num) {
    return num.at(0) == '0';
}

string numUtil::removeLeadingZeros(const string& num) {
    if (!hasLeadingZeros(num)) {
        return num;
    } else {
        string temp = strUtil::removeLeadingCharacters(num, '0');
        return temp.empty() ? "0" : temp;
    }
}

// removes leading 0's (except for 1 if before decimal point), adds leading 0 if decimal starts with a dot, removes redundant + sign
// removes trailing decimal point and trailing 0's after decimal point, turns -0 into 0
string numUtil::simplify(const string& num) {
    if (num == "" || num == ".") {
        return "0";
    }
    if (hasTrailingZeros(num)) {
        return simplify(removeTrailingZeros(num));
    }
    if (num.back() == '.') {
        return simplify(num.substr(0, num.size() - 1));
    }
    if (num.at(0) == '+') {
        return simplify(num.substr(1));
    }
    if (num.at(0) == '-') {
        string allElse = num.substr(1);
        if (strUtil::removeAllOccurrencesOfChar(allElse, '0') == "") {
            return "0";
        } else {
            return "-" + simplify(allElse);
        }
    }
    if (!strUtil::contains(num, ".")) {
        if (hasLeadingZeros(num)) {
            return removeLeadingZeros(num);
        } else {
            return num;
        }
    } else {
        string almostProcessed;
        if (hasLeadingZeros(num)) {
            almostProcessed = removeLeadingZeros(num);
        } else {
            almostProcessed = num;
        }
        if (almostProcessed.at(0) == '.') {
            return "0" + almostProcessed;
        } else {
            return almostProcessed;
        }
    }
}