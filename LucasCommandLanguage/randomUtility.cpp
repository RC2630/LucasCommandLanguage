#include "randomUtility.h"

RandUtil::RandUtil()
    : initialized(false)
{ }

int RandUtil::randint(int a, int b) {
    if (!initialized) {
        init();
    }
    return rand() % (b - a + 1) + a;
}

// highly recommended to not directly call this function from outside this file (not like you can anyways now that it is private)
void RandUtil::init() {
    if (initialized) {
        throw runtime_error("init() has already been called before");
    } else {
        srand(time(nullptr));
        initialized = true;
    }
}