#include "hashing.h"

// A function which calculates the hash factor in the beginning,
// to prevent calculate it again and again.
long long getFactor(int k)
{
    long long factor = 1;

    for (int i = 0; i < k; i++)
        factor = (factor * PRIME_BASE) % PRIME_MOD;

    return factor;
}

// A function which implements positive modula operation.
long long positiveMod(long long x, long long y)
{
    long long ans = x % y;
    return ans >= 0 ? ans : ans + y;
}

// A function which gets hash keys from code skeleton with Karp-Rabin algorithm.
std::vector <long long> karpRabinHashing(const std::vector <SkeletonPiece> &skeleton, int k, long long factor)
{
    std::vector <long long> hashKeys;

    long long key = 0;

    for (size_t i = 0; i < k; ++i)
        key = (key*PRIME_BASE + skeleton[i].ch) % PRIME_MOD;

    hashKeys.push_back(key);

    for (size_t i = k; i < skeleton.size(); i++)
    {
        key = positiveMod(key*PRIME_BASE + skeleton[i].ch - factor*skeleton[i-k].ch, PRIME_MOD);
        hashKeys.push_back(key);
    }

    return hashKeys;
}
