#include "binaryUtility.h"

vector<int> binUtil::decToBin1Byte(int dec) {
    if (dec < 0) {
        return twosComplement(decToBin1Byte(-dec));
    }
    vector<int> v;
    for (int i = 7; i >= 0; i--) {
        if (dec - pow(2, i) >= 0) {
            v.push_back(1);
            dec -= pow(2, i);
        } else {
            v.push_back(0);
        }
    }
    return v;
}

int binUtil::binToDec1Byte(const vector<int>& bin, bool isUnsigned) {
    if (!isUnsigned && bin.at(0) == 1) {
        return -binToDec1Byte(twosComplement(bin));
    }
    int d = 0;
    for (int i = 0; i < bin.size(); i++) {
        if (bin.at(i) == 1) {
            d += pow(2, 7 - i);
        }
    }
    return d;
}

vector<int> binUtil::decToBinNoLeading0(int dec) {
    if (dec < 0) {
        throw invalid_argument("negative numbers not supported for the no leading 0's conversion; use a different conversion function");
    }
    if (dec == 0) {
        return {0};
    }
    vector<int> v;
    bool hasLeading1 = false;
    for (int i = numUtil::log(dec, 2); i >= 0; i--) {
        if (dec - pow(2, i) >= 0) {
            v.push_back(1);
            dec -= pow(2, i);
            hasLeading1 = true;
        } else if (hasLeading1) {
            v.push_back(0);
        }
    }
    return v;
}

int binUtil::minRequiredDigits(int dec) {
    if (dec < 0) {
        throw invalid_argument("not supported for negative numbers; try finding the min required digits for a positive number, ex. its abs value");
    }
    return (dec == 0) ? 1 : (numUtil::log(dec, 2) + 1);
}

vector<int> binUtil::decToBinNdigits(int dec, int n) {
    if (dec < 0) {
        vector<int> bin = twosComplement(decToBinNdigits(-dec, n));
        if (bin.at(0) == 0) {
            throw invalid_argument("you need 1 extra digit because your number is negative");
        }
        return bin;
    }
    int minRequired = minRequiredDigits(dec);
    if (n < minRequired) {
        throw invalid_argument("minimum required digits = " + to_string(minRequired));
    }
    vector<int> v;
    for (int i = n - 1; i >= 0; i--) {
        if (dec - pow(2, i) >= 0) {
            v.push_back(1);
            dec -= pow(2, i);
        } else {
            v.push_back(0);
        }
    }
    return v;
}

string binUtil::displayGroupsOf(const vector<int>& binaryNum, int digitsPerGroup, bool fillToFullGroup, bool isUnsigned) {
    string fill = "0";
    if (!isUnsigned && binaryNum.at(0) == 1) {
        fill = "1";
    }
    string disp;
    int i, numDigitsDisplayed;
    for (i = binaryNum.size() - 1, numDigitsDisplayed = 0; i >= 0; i--, numDigitsDisplayed++) {
        if (numDigitsDisplayed % digitsPerGroup == 0 && numDigitsDisplayed != 0) {
            disp = " " + disp;
        }
        disp = to_string(binaryNum.at(i)) + disp;
    }
    if (fillToFullGroup && binaryNum.size() % digitsPerGroup != 0) {
        for (int i = 1; i <= digitsPerGroup - binaryNum.size() % digitsPerGroup; i++) {
            disp = fill + disp;
        }
    }
    return disp;
}

int binUtil::binToDec(const vector<int>& bin, bool isUnsigned) {
    if (!isUnsigned && bin.at(0) == 1) {
        return -binToDec(twosComplement(bin));
    }
    int d = 0;
    int i, power;
    for (i = bin.size() - 1, power = 0; i >= 0; i--, power++) {
        if (bin.at(i) == 1) {
            d += pow(2, power);
        }
    }
    return d;
}

vector<int> binUtil::onesComplement(const vector<int>& bin) {
    vector<int> result;
    for (int digit : bin) {
        result.push_back(!digit);
    }
    return result;
}

vector<int> binUtil::twosComplement(const vector<int>& bin) {
    if (binToDec(bin) == 0) {
        return bin;
    } else {
        return decToBinNdigits(binToDec(onesComplement(bin)) + 1, bin.size());
    }
}

// anything you can do with two integers, you can also do with binary numbers, supposedly; this function helps you do exactly that
// NOTE: result will be cast into the given number of digits even when information loss occurs
// NOTE: use the default argument for number of digits of the result binary number if you want it to be trimmed (this prevents information loss)
vector<int> binUtil::arithmetic(const vector<int>& operand1, const vector<int>& operand2,
    int (*operation) (int, int), bool isUnsigned, int numDigits) {
    int operand1_decimal = binToDec(operand1, isUnsigned);
    int operand2_decimal = binToDec(operand2, isUnsigned);
    int result = operation(operand1_decimal, operand2_decimal);
    vector<int> safe = decToBinNdigits(result, minRequiredDigits(abs(result)) + 4); // the "safety" 4 digits will not affect the returned result
    if (numDigits == -1) {
        return trim(safe, isUnsigned);
    } else {
        return cast(safe, numDigits, isUnsigned);
    }
}

vector<int> binUtil::arithmetic_f(const vector<int>& operand1, const vector<int>& operand2,
    function<int(int, int)> operation, bool isUnsigned, int numDigits) {
    int operand1_decimal = binToDec(operand1, isUnsigned);
    int operand2_decimal = binToDec(operand2, isUnsigned);
    int result = operation(operand1_decimal, operand2_decimal);
    vector<int> safe = decToBinNdigits(result, minRequiredDigits(abs(result)) + 4); // the "safety" 4 digits will not affect the returned result
    if (numDigits == -1) {
        return trim(safe, isUnsigned);
    } else {
        return cast(safe, numDigits, isUnsigned);
    }
}

// casts the binary number into the given number of digits; this could result in information loss, just like real casting
// NOTE: it is possible that when information loss occurs, a positive number becomes a negative number and vice versa
// NOTE: when signed, proper sign extension will be used
vector<int> binUtil::cast(const vector<int>& bin, int numDigits, bool isUnsigned) {
    if (numDigits == bin.size()) {
        return bin;
    } else if (numDigits < bin.size()) {
        return vecUtil::subvector(bin, bin.size() - numDigits, bin.size() - 1);
    } else {
        vector<int> extraDigits;
        int fill = 0;
        if (!isUnsigned && bin.at(0) == 1) {
            fill = 1;
        }
        for (int i = 1; i <= numDigits - bin.size(); i++) {
            extraDigits.push_back(fill);
        }
        return vecUtil::concatenate<int>({extraDigits, bin});
    }
}

// trims the binary number into the same number but without all leading unnecessary digits (extra 0's if unsigned, extra 1's if signed)
vector<int> binUtil::trim(const vector<int>& binParam, bool isUnsigned) {
    vector<int> bin = binParam;
    if (!isUnsigned && bin.at(0) == 1) {
        // signed negative trim: get rid of all leading 1's except for one
        while (bin.at(0) == 1) {
            vecUtil::removeFirstInstance(bin, 1);
        }
        return vecUtil::concatenate<int>({{1}, bin});
    } else if (!isUnsigned && bin.at(0) == 0) {
        // signed positive trim: get rid of all leading 0's except for one
        while (bin.at(0) == 0) {
            vecUtil::removeFirstInstance(bin, 0);
        }
        return vecUtil::concatenate<int>({{0}, bin});
    } else {
        // unsigned trim: get rid of all leading 0's
        while (bin.at(0) == 0) {
            vecUtil::removeFirstInstance(bin, 0);
        }
        return bin;
    }
}

// returns {MAX_UNSIGNED, MIN_SIGNED, MAX_SIGNED} in decimal (base 10) for the given number of digits
vector<int> binUtil::bounds(int numDigits) {
    int maxUnsigned = pow(2, numDigits) - 1;
    int minSigned = -pow(2, numDigits - 1);
    int maxSigned = pow(2, numDigits - 1) - 1;
    return {maxUnsigned, minSigned, maxSigned};
}

vector<int> binUtil::unsignedToSigned(const vector<int>& unsignedBin) {
    if (unsignedBin.at(0) == 0) {
        return unsignedBin;
    } else {
        return vecUtil::concatenate<int>({{0}, unsignedBin});
    }
}

// input must only contain the following characters: '1', '0', ' '
vector<int> binUtil::strToBin(const string& str) {
    string noSpaces = strUtil::removeChars(str, ' ', strUtil::numOccurrences(str, ' '));
    vector<int> result;
    for (char c : noSpaces) {
        result.push_back(c - 48);
    }
    return result;
}