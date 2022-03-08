#include "randomUtility.h"

RandUtil::RandUtil() {
    srand(time(nullptr));
}

int RandUtil::randint(int a, int b) {
    return rand() % (b - a + 1) + a;
}