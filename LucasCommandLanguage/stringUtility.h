#ifndef STR_UTIL_DOT_H
#define STR_UTIL_DOT_H

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;

namespace strUtil {

    string charToString(char c) {
        return string(1, c);
    }

    bool contains(const string& s, const string& subs) {
        return s.find(subs) != string::npos;
    }

    bool containsAtLeast1Of(const string& s, const vector<string>& v) {
        for (const string& e : v) {
            if (contains(s, e)) {
                return true;
            }
        }
        return false;
    }

    bool beginsWith(const string& s, const string& subs) {
        return s.find(subs) == 0;
    }

    string removeChars(const string& s, char toRemove, int num) {
        string newString;
        for (char c : s) {
            if (c == toRemove && num > 0) {
                // nothing appended to newString
                num--;
            } else {
                newString += c;
            }
        }
        return newString;
    }

    string removeAllAfterChar(const string& s, char remAfter) {
        if (!contains(s, charToString(remAfter))) {
            return s;
        }
        string newString;
        for (char c : s) {
            if (c != remAfter) {
                newString += c;
            } else {
                break;
            }
        }
        return newString;
    }

    // returns the position in s of the final occurrence of c (throws exception if c is not in s)
    int positionOfFinalOccurrence(const string& s, char c) {
        for (int i = s.size() - 1; i >= 0; i--) {
            if (s.at(i) == c) {
                return i;
            }
        }
        throw runtime_error(string() + "'" + c + "' is not in \"" + s + "\"");
    }

    int numOccurrences(const string& s, char c) {
        return count(s.begin(), s.end(), c);
    }

    bool parseBool(const string& s) {
        return s == "true" || s == "True" || s == "TRUE";
    }

    string boolval(bool b) {
        return b ? "true" : "false";
    }

    // requires matching open/close delimiters and no nested delimiters, also requires open delimiter to be different from close delimiter
    // if delimiters are not matched, may run with unexpected results OR throw runtime_error
    // (generally, missing open delimiters will run with unexpected results, while missing close delimiters will throw)
    // if delimiters are nested, may still run but will produce unexpected results
    // if open delimiter and close delimiter are the same character, may still run but will produce unexpected results
    // MORAL: JUST FOLLOW THE REQUIREMENTS!
    // common delimiter pairs used: {} () [] <>
    vector<string> partsSplitByOpenCloseDelimiters(const string& s, char openDelimiter, char closeDelimiter) {
        vector<string> parts;
        if (!contains(s, charToString(openDelimiter))) {
            parts.push_back(s);
            return parts;
        }
        string currentString;
        int i = 0;
        while (true) {
            if (s.at(i) == openDelimiter) {
                for (int r = i; true; r++) {
                    if (r >= s.size()) {
                        string errorMessage = "mismatched delimiters - no closing delimiter ";
                        errorMessage += closeDelimiter;
                        errorMessage += " found";
                        throw runtime_error(errorMessage);
                    }
                    currentString += s.at(r);
                    if (s.at(r) == closeDelimiter) {
                        i = r + 1;
                        parts.push_back(currentString);
                        currentString = "";
                        break;
                    }
                }
            } else {
                for (int r = i; true; r++) {
                    if (r < s.size() && s.at(r) != openDelimiter) {
                        currentString += s.at(r);
                    } else {
                        i = r;
                        parts.push_back(currentString);
                        currentString = "";
                        break;
                    }
                }
            }
            if (i >= s.size()) {
                break;
            }
        }
        return parts;
    }

    void replaceAllChar1WithChar2(string& s, char char1, char char2) {
        for (int i = 0; i < s.size(); i++) {
            if (s.at(i) == char1) {
                s.at(i) = char2;
            }
        }
    }

    string spaces(int n) {
        string s;
        for (int i = 1; i <= n; i++) {
            s += " ";
        }
        return s;
    }

    // returns 0 if v is empty
    int maxStringLength(const vector<string>& v) {
        int max = 0;
        for (const string& s : v) {
            if (s.size() > max) {
                max = s.size();
            }
        }
        return max;
    }

    // returns 0 if v is empty
    int minStringLength(const vector<string>& v) {
        int min = maxStringLength(v);
        for (const string& s : v) {
            if (s.size() < min) {
                min = s.size();
            }
        }
        return min;
    }

}

#endif