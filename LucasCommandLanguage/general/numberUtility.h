#ifndef NUM_UTIL_DOT_H
#define NUM_UTIL_DOT_H

#include <cmath>
#include <string>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <functional>
#include <numeric>

#include "abstractFunctions.h"
#include "stringUtility.h"

using namespace std;

// READ CAREFULLY BEFORE USING THE FUNCTIONS IN THIS FILE:
// 
// the functions here operate primarily on floating-point numbers, and so we need to test very carefully for round-off errors.
// 
// any of the functions here that are guaranteed to not behave erratically due to round-off errors, up to 14 decimal places, are
// marked with the special comment: "ROEPCF" (stands for "Round-Off-Error-Proof Certified Function").
// 
// note that beyond 14 decimal places, there can still be errors, but that is beyond our control due to system hardware limitations.
// this means if input with > 14 decimal places is supplied to an ROEPCF-marked function, strange behaviour can still be expected.
// 
// functions marked with "ROEPCF" can still behave strangely if there are additional conditions, requirements,
// or warnings explicitly marked in the function's comments.
// 
// for example, if the function says "does not work well with numbers 10^12 or larger", then ROEPCF does not guarantee precision,
// accuracy, or correctness for any input 10^12 or larger.

namespace numUtil {

    const double PI = 3.141592654;
    const double EULER_NUM = 2.718281828;
    const double pi = PI, e = EULER_NUM; // alternative names

    // ROEPCF
    double round(double value, int numPlaces);

    // ROEPCF
    // does not work correctly for big numbers (i.e. absolute value of d is on the order of 10^10 or bigger)
    // works correctly for small numbers though (i.e. absolute value of d is on the order of 10^-10 or smaller)
    bool doubleIsAnInteger(double d);

    // ROEPCF
    // rounds to the nearest integer, but if there are 2 integers equal distance away from value, then return the bigger one
    // this means 1.5 -> 2 (because 2 is bigger than 1), and -1.5 -> -1 (because -1 is bigger than -2)
    int roundToNearestInt(double value);

    // ROEPCF
    // requires d to be strictly positive
    // requires base >= 2
    // this function returns true if base^k = d for some integer k
    // note: when d < base and d != 1, this function returns true if base^-n = d, for some positive integer n
    //       and when d = 1, this function always returns true because base^0 = 1, and 0 is an integer
    // example: isExactPowerOf(n, 2) will return true for any n in the set {..., 16, 8, 4, 2, 1, 0.5, 0.25, 0.125, 0.0625, ...}
    //          because there exists an integer i such that 2^i = n for all n in the above set
    //          (..., 2^4 = 16, 2^3 = 8, 2^2 = 4, 2^1 = 2, 2^0 = 1, 2^-1 = 0.5, 2^-2 = 0.25, 2^-3 = 0.125, 2^-4 = 0.0625, ...)
    bool isExactPowerOf(double d, int base);

    // ROEPCF
    // requires d to be non-negative
    int numCharBeforeDot(double d);

    // ROEPCF
    // warning: if abs(n) is particularly big, or n has particularly many decimal places of precision, the result of this function can suffer
    //          in precision (< 14 decimal digits of precision). However, this is the same as what happens if you do this in any other
    //          programming language, because this is just another effect of hardware limitations
    // gets the i-th decimal place digit of n and return it as a character
    char digitOf(double n, int i);

    // ROEPCF
    // warning: see warning for digitOf(), since this function uses digitOf()
    // converts given double to its string representation with 14 decimal places
    string doubleToString(double d);

    // ROEPCF
    // warning: see warning for digitOf(), since this function uses doubleToString(), which in turn uses digitOf()
    string toStringRemoveTrailingZeros(double num);

    // EVERYTHING AFTER THIS POINT IS AUTOMATICALLY ROEPCF, UNLESS OTHERWISE SPECIFIED
    double log(double arg, double base);
    double radToDeg(double rad);
    double degToRad(double deg);
    double degreeMode(double (*trigFuncRad) (double), double deg);
    double degreeMode_f(function<double(double)> trigFuncRad, double deg);
    int factorial(int n);
    int numPermutations(int total, int choose);
    int numCombinations(int total, int choose);
    int multinomialCoefficient(int n, const vector<int>& ks);

    // THE FOLLOWING FUNCTIONS WORK WITH STRING REPRESENTATIONS OF DECIMAL NUMBERS
    // NOTE: THEY ALL SIMPLIFY num BEFORE PROCEEDING
    int numDecimalPlaces(const string& num);
    string roundToNplaces(const string& num, int n);

    // THESE ARE ALL HELPERS FOR simplify(), YOU ARE RECOMMENDED TO NOT USE THEM DIRECTLY
    bool hasTrailingZeros(const string& num);
    string removeTrailingZeros(const string& num);
    bool hasLeadingZeros(const string& num);
    string removeLeadingZeros(const string& num);

    // removes leading 0's (except for 1 if before decimal point), adds leading 0 if decimal starts with a dot, removes redundant + sign
    // removes trailing decimal point and trailing 0's after decimal point, turns -0 into 0
    string simplify(const string& num);

}

#endif