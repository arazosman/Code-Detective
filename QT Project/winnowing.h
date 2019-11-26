#ifndef WINNOWING_H
#define WINNOWING_H

#include <algorithm>

#include "fundamentals.h"
#include "hashing.h"

void getFingerprints(Code &code, int k, int w, long long factor);
int compareCodes(Code &code1, Code &code2);

#endif // WINNOWING_H
