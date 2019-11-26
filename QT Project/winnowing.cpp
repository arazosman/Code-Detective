#include "winnowing.h"

// A function which gets fingerprints of a code.
// k: fingerprint length
// w: window size
// factor: hash factor
void getFingerprints(Code &code, int k, int w, long long factor)
{
    // Applying Karp-Rabin algorithm:
    std::vector <long long> hashKeys = karpRabinHashing(code.skeleton, k, factor);

    // Applying Winnowing algorithm:
    int min = -1; // index of minimum hash

    if (hashKeys.size() < w)
    {
        min = std::min_element(hashKeys.begin(), hashKeys.end()) - hashKeys.begin();
        code.fingerprints[hashKeys[min]].push_back({code.skeleton[min].lineNo, code.skeleton[min+k-1].lineNo});
        ++code.numOfSelectedFingerPrints;
    }
    else
    {
        for (int i = 0; i < hashKeys.size()-w+1; ++i)
        {
            if (min < i) // the previous minimum hash key is no longer in the window
            {
                min = std::min_element(hashKeys.begin()+i, hashKeys.begin()+i+w) - hashKeys.begin();
                code.fingerprints[hashKeys[min]].push_back({code.skeleton[min].lineNo, code.skeleton[min+k-1].lineNo});
                ++code.numOfSelectedFingerPrints;
            }
            else if (hashKeys[i+w-1] <= hashKeys[min]) // the previous minimum hash key is still in the window
            {
                min = i+w-1;
                code.fingerprints[hashKeys[min]].push_back({code.skeleton[min].lineNo, code.skeleton[min+k-1].lineNo});
                ++code.numOfSelectedFingerPrints;
            }
        }
    }
}

// A function which compares a given code pair.
int compareCodes(Code &code1, Code &code2)
{
    int tokens_matched = 0; // number of matched fingerprints
    Code *firstCode, *secondCode;

    // selecting the code which has less fingerprints as the first code
    if (code1.numOfSelectedFingerPrints < code2.numOfSelectedFingerPrints)
        firstCode = &code1, secondCode = &code2;
    else
        firstCode = &code2, secondCode = &code1;

    // since the first code has less fingerprints, less calculations
    // will be made for the comparison below:
    for (const auto &p1: firstCode->fingerprints)
    {
        auto p2 = secondCode->fingerprints.find(p1.first);

        // if there is a match for curent fingerprint, then number of matched
        // fingerprints will be increased as the number of that fingerprint
        if (p2 != secondCode->fingerprints.end())
            tokens_matched += std::min(p1.second.size(), p2->second.size());
    }

    return tokens_matched;
}
