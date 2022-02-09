#ifndef STR_UTIL_DOT_H
#define STR_UTIL_DOT_H

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;

namespace strUtil {
    string charToString(char c);
    bool contains(const string& s, const string& subs);
    bool containsAtLeast1Of(const string& s, const vector<string>& v);
    bool beginsWith(const string& s, const string& subs);
    string reverse(const string& s);
    string sort(const string& s);
    bool endsWith(const string& s, const string& subs);
    string removeChars(const string& s, char toRemove, int num);
    string removeAllAfterChar(const string& s, char remAfter);
    // returns the position in s of the first occurrence of c (throws exception if c is not in s)
    int positionOfFirstOccurrence(const string& s, char c);
    // returns the position in s of the final occurrence of c (throws exception if c is not in s)
    int positionOfFinalOccurrence(const string& s, char c);
    vector<int> positionsOfAllOccurrences(const string& s, char c);
    string removeAllBeforeChar(const string& s, char remBefore);
    int numOccurrences(const string& s, char c);
    string removeAllOccurrencesOfChar(const string& s, char c);
    bool parseBool(const string& s);
    string boolval(bool b);
    // requires matching open/close delimiters and no nested delimiters, also requires open delimiter to be different from close delimiter
    // if delimiters are not matched, may run with unexpected results OR throw runtime_error
    // (generally, missing open delimiters will run with unexpected results, while missing close delimiters will throw)
    // if delimiters are nested, may still run but will produce unexpected results
    // if open delimiter and close delimiter are the same character, may still run but will produce unexpected results
    // MORAL: JUST FOLLOW THE REQUIREMENTS!
    // common delimiter pairs used: {} () [] <>
    vector<string> partsSplitByOpenCloseDelimiters(const string& s, char openDelimiter, char closeDelimiter);
    void replaceAllChar1WithChar2(string& s, char char1, char char2);
    string spaces(int n);
    // returns 0 if v is empty
    int maxStringLength(const vector<string>& v);
    // returns 0 if v is empty
    int minStringLength(const vector<string>& v);
    string removeLeadingSpaces(const string& s);
    string removeTrailingSpaces(const string& s);
    string trim(const string& s);
    bool containsIgnoreLeadingSpaces(const vector<string>& v, const string& s);
    bool containsIgnoreTrailingSpaces(const vector<string>& v, const string& s);
    bool containsTrimmed(const vector<string>& v, const string& s);
    vector<string> fillSpacesTo(const vector<string>& v, int size);
    vector<string> fillSpacesToMaxPlusN(const vector<string>& v, int n = 0);
    vector<string> quotify(const vector<string>& v);
}

#endif