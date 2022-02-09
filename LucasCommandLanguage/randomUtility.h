#ifndef RANDOM_DOT_H
#define RANDOM_DOT_H

#include <ctime>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <stdexcept>

using namespace std;

class RandUtil {

    // do not touch this variable from outside this file (not like you can anyways now that it is private)
    bool initialized;

public:

    RandUtil();
    int randint(int a, int b);

    // returns a random element from v (by copy)
    template <typename T>
    T randomElement(const vector<T>& v) {
        if (v.empty()) {
            throw invalid_argument("v cannot be empty");
        }
        return v.at(randint(0, v.size() - 1));
    }

    // returns a random element from v (by reference)
    template <typename T>
    T& randomElementRef(vector<T>& v) {
        if (v.empty()) {
            throw invalid_argument("v cannot be empty");
        }
        return v.at(randint(0, v.size() - 1));
    }

private:

    // highly recommended to not directly call this function from outside this file (not like you can anyways now that it is private)
    void init();

};

#endif