#include "stringUtility.h"

string strUtil::charToString(char c) {
    return string(1, c);
}

bool strUtil::contains(const string& s, const string& subs) {
    return s.find(subs) != string::npos;
}

bool strUtil::containsAtLeast1Of(const string& s, const vector<string>& v) {
    for (const string& e : v) {
        if (contains(s, e)) {
            return true;
        }
    }
    return false;
}

bool strUtil::beginsWith(const string& s, const string& subs) {
    return s.find(subs) == 0;
}

string strUtil::reverse(const string& s) {
    string reversed = s;
    std::reverse(reversed.begin(), reversed.end());
    return reversed;
}

string strUtil::sort(const string& s) {
    string sorted = s;
    std::sort(sorted.begin(), sorted.end());
    return sorted;
}

bool strUtil::endsWith(const string& s, const string& subs) {
    return beginsWith(reverse(s), reverse(subs));
}

string strUtil::removeChars(const string& s, char toRemove, int num) {
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

// removes everything before (but NOT including, by default) the first occurrence of remBefore in s
string strUtil::removeAllBeforeChar(const string& s, char remBefore, bool remCharItself) {
    if (!contains(s, charToString(remBefore))) {
        return s;
    }
    if (remCharItself) {
        return s.substr(positionOfFirstOccurrence(s, remBefore) + 1);
    } else {
        return s.substr(positionOfFirstOccurrence(s, remBefore));
    }
}

// removes everything after (and including, by default) the first occurrence of remAfter in s
string strUtil::removeAllAfterChar(const string& s, char remAfter, bool remCharItself) {
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
    if (remCharItself) {
        return newString;
    } else {
        return newString + remAfter;
    }
}

// returns the position in s of the first occurrence of c (throws exception if c is not in s)
int strUtil::positionOfFirstOccurrence(const string& s, char c) {
    for (int i = 0; i < s.size(); i++) {
        if (s.at(i) == c) {
            return i;
        }
    }
    throw runtime_error(string() + "'" + c + "' is not in \"" + s + "\"");
}

// returns the position in s of the final occurrence of c (throws exception if c is not in s)
int strUtil::positionOfFinalOccurrence(const string& s, char c) {
    for (int i = s.size() - 1; i >= 0; i--) {
        if (s.at(i) == c) {
            return i;
        }
    }
    throw runtime_error(string() + "'" + c + "' is not in \"" + s + "\"");
}

vector<int> strUtil::positionsOfAllOccurrences(const string& s, char c) {
    vector<int> positions;
    for (int i = 0; i < s.size(); i++) {
        if (s.at(i) == c) {
            positions.push_back(i);
        }
    }
    return positions;
}

int strUtil::numOccurrences(const string& s, char c) {
    return count(s.begin(), s.end(), c);
}

string strUtil::removeAllOccurrencesOfChar(const string& s, char c) {
    return removeChars(s, c, numOccurrences(s, c));
}

bool strUtil::parseBool(const string& s) {
    return s == "true" || s == "True" || s == "TRUE";
}

string strUtil::boolval(bool b) {
    return b ? "true" : "false";
}

// requires matching open/close delimiters and no nested delimiters, also requires open delimiter to be different from close delimiter
// if delimiters are not matched, may run with unexpected results OR throw runtime_error
// (generally, missing open delimiters will run with unexpected results, while missing close delimiters will throw)
// if delimiters are nested, may still run but will produce unexpected results
// if open delimiter and close delimiter are the same character, may still run but will produce unexpected results
// MORAL: JUST FOLLOW THE REQUIREMENTS!
// common delimiter pairs used: {} () [] <>
vector<string> strUtil::partsSplitByOpenCloseDelimiters(const string& s, char openDelimiter, char closeDelimiter) {
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

void strUtil::replaceAllChar1WithChar2(string& s, char char1, char char2) {
    for (int i = 0; i < s.size(); i++) {
        if (s.at(i) == char1) {
            s.at(i) = char2;
        }
    }
}

string strUtil::spaces(int n) {
    string s;
    for (int i = 1; i <= n; i++) {
        s += " ";
    }
    return s;
}

// returns 0 if v is empty
int strUtil::maxStringLength(const vector<string>& v) {
    int max = 0;
    for (const string& s : v) {
        if (s.size() > max) {
            max = s.size();
        }
    }
    return max;
}

// returns 0 if v is empty
int strUtil::minStringLength(const vector<string>& v) {
    int min = maxStringLength(v);
    for (const string& s : v) {
        if (s.size() < min) {
            min = s.size();
        }
    }
    return min;
}

string strUtil::removeLeadingCharacters(const string& s, char c) {
    int indexFirstNonCharacter = 0;
    for (int pos = 0; pos < s.size(); pos++) {
        if (s.at(pos) == c) {
            indexFirstNonCharacter++;
        } else {
            break;
        }
    }
    return s.substr(indexFirstNonCharacter);
}

string strUtil::removeTrailingCharacters(const string& s, char c) {
    int indexLastNonCharacter = s.size() - 1;
    for (int pos = s.size() - 1; pos >= 0; pos--) {
        if (s.at(pos) == c) {
            indexLastNonCharacter--;
        } else {
            break;
        }
    }
    return s.substr(0, indexLastNonCharacter + 1);
}

string strUtil::removeLeadingSpaces(const string& s) {
    return removeLeadingCharacters(s, ' ');
}

string strUtil::removeTrailingSpaces(const string& s) {
    return removeTrailingCharacters(s, ' ');
}

string strUtil::trim(const string& s) {
    return removeLeadingSpaces(removeTrailingSpaces(s));
}

bool strUtil::containsIgnoreLeadingSpaces(const vector<string>& v, const string& s) {
    string sWithoutLeadingSpaces = removeLeadingSpaces(s);
    for (const string& e : v) {
        if (removeLeadingSpaces(e) == sWithoutLeadingSpaces) {
            return true;
        }
    }
    return false;
}

bool strUtil::containsIgnoreTrailingSpaces(const vector<string>& v, const string& s) {
    string sWithoutTrailingSpaces = removeTrailingSpaces(s);
    for (const string& e : v) {
        if (removeTrailingSpaces(e) == sWithoutTrailingSpaces) {
            return true;
        }
    }
    return false;
}

bool strUtil::containsTrimmed(const vector<string>& v, const string& s) {
    string sTrimmed = trim(s);
    for (const string& e : v) {
        if (trim(e) == sTrimmed) {
            return true;
        }
    }
    return false;
}

vector<string> strUtil::fillSpacesTo(const vector<string>& v, int size) {
    vector<string> filled = v;
    for (string& s : filled) {
        s += spaces(size - s.size());
    }
    return filled;
}

vector<string> strUtil::fillSpacesToMaxPlusN(const vector<string>& v, int n) {
    return fillSpacesTo(v, maxStringLength(v) + n);
}

string strUtil::quotify(const string& s) {
    return "\"" + s + "\"";
}

vector<string> strUtil::quotify(const vector<string>& v) {
    vector<string> quoted;
    for (const string& s : v) {
        quoted.push_back(strUtil::quotify(s));
    }
    return quoted;
}

vector<string> strUtil::spaceSplit(const string& s) {
    vector<string> v;
    string temp;
    for (char c : s) {
        if (c == ' ') {
            if (!temp.empty()) {
                v.push_back(temp);
                temp.clear();
            }
        } else {
            temp += c;
        }
    }
    if (!temp.empty()) {
        v.push_back(temp);
    }
    return v;
}

string strUtil::toUpper(const string& s) {
    string n;
    for (char c : s) {
        n += (char) toupper(c);
    }
    return n;
}

string strUtil::toLower(const string& s) {
    string n;
    for (char c : s) {
        n += (char) tolower(c);
    }
    return n;
}

bool strUtil::equalsIgnoreCase(const string& s1, const string& s2) {
    return toUpper(s1) == toUpper(s2);
}

bool strUtil::isLetter(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

bool strUtil::isLowerCaseLetter(char c) {
    return 'a' <= c && c <= 'z';
}

bool strUtil::isUpperCaseLetter(char c) {
    return 'A' <= c && c <= 'Z';
}

bool strUtil::isDigit(char c) {
    return '0' <= c && c <= '9';
}

// separates a string into a list of words and spaces/punctuation, ex. "I am. Are you?" -> {"I", " ", "am", ". ", "Are", " ", "you", "?"}
vector<string> strUtil::separateIntoWordsAndSpaces(const string& s) {
    vector<string> parts;
    bool wordMode = true;
    string curr;
    for (char c : s) {
        if ((strUtil::isLetter(c) && wordMode) || (!strUtil::isLetter(c) && !wordMode)) {
            curr += c;
        } else {
            wordMode = !wordMode;
            parts.push_back(curr);
            curr = c;
        }
    }
    parts.push_back(curr);
    return parts;
}

vector<int> strUtil::getAsciiFromString(const string& s) {
    vector<int> points;
    for (char c : s) {
        points.push_back((std::uint8_t) c);
    }
    return points;
}