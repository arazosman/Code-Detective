#ifndef HASHING_H
#define HASHING_H

#include <iostream>
#include <string>
#include <vector>

#include "fundamentals.h"

const unsigned PRIME_BASE = 257;
const unsigned PRIME_MOD = 1e9+7;

long long getFactor(int k);
long long positiveMod(long long x, long long y);
std::vector <long long> karpRabinHashing(const std::vector <SkeletonPiece> &skeleton, int k, long long factor);

#endif
