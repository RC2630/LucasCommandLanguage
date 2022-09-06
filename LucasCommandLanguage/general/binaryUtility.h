#ifndef BINARY_UTILITY_DOT_H
#define BINARY_UTILITY_DOT_H

#include "numberUtility.h"
#include "stringUtility.h"
#include "vectorUtility.h"

using namespace std;

namespace binUtil {
    vector<int> decToBin1Byte(int dec);
    int binToDec1Byte(const vector<int>& bin, bool isUnsigned = true);
    vector<int> decToBinNoLeading0(int dec);
    int minRequiredDigits(int dec);
    vector<int> decToBinNdigits(int dec, int n);
    string displayGroupsOf(const vector<int>& binaryNum, int digitsPerGroup = 8, bool fillToFullGroup = false, bool isUnsigned = true);
    int binToDec(const vector<int>& bin, bool isUnsigned = true);
    vector<int> onesComplement(const vector<int>& bin);
    vector<int> twosComplement(const vector<int>& bin);
    // anything you can do with two integers, you can also do with binary numbers, supposedly; this function helps you do exactly that
    // NOTE: result will be cast into the given number of digits even when information loss occurs
    // NOTE: use the default argument for number of digits of the result binary number if you want it to be trimmed (this prevents information loss)
    vector<int> arithmetic(const vector<int>& operand1, const vector<int>& operand2,
        int (*operation) (int, int), bool isUnsigned = true, int numDigits = -1);
    vector<int> arithmetic_f(const vector<int>& operand1, const vector<int>& operand2,
        function<int(int, int)> operation, bool isUnsigned = true, int numDigits = -1);
    // casts the binary number into the given number of digits; this could result in information loss, just like real casting
    // NOTE: it is possible that when information loss occurs, a positive number becomes a negative number and vice versa
    // NOTE: when signed, proper sign extension will be used
    vector<int> cast(const vector<int>& bin, int numDigits, bool isUnsigned);
    // trims the binary number into the same number but without all leading unnecessary digits (extra 0's if unsigned, extra 1's if signed)
    vector<int> trim(const vector<int>& binParam, bool isUnsigned);
    // returns {MAX_UNSIGNED, MIN_SIGNED, MAX_SIGNED} in decimal (base 10) for the given number of digits
    vector<int> bounds(int numDigits);
    vector<int> unsignedToSigned(const vector<int>& unsignedBin);
    // input must only contain the following characters: '1', '0', ' '
    vector<int> strToBin(const string& str);
}

#endif