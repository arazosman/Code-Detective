#ifndef FUNDAMENTALS_H
#define FUNDAMENTALS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct Line
{
    int beg, end; // beginning and ending lines
};

struct SkeletonPiece
{
    char ch;
    int lineNo;
};

struct Code
{
    std::vector <SkeletonPiece> skeleton;
    std::string fullCode, pureCode, filePath, fileName;
    std::unordered_map <int, std::vector <Line>> fingerprints;
    int numOfSelectedFingerPrints = 0;
};

#endif // FUNDAMENTALS_H
