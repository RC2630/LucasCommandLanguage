#ifndef RANDOM_DOT_H
#define RANDOM_DOT_H

#include <ctime>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <stdexcept>

using namespace std;

struct RandUtil {

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

};

#endif