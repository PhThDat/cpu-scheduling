#ifndef UTILS_H
#define UTILS_H

#include "stdlib.h"

int randomBetween(int min, int max) {
    return rand() % (max - min + 1) + min;
}

#endif