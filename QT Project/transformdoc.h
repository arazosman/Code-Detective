#ifndef TRANSFORMDOC_H
#define TRANSFORMDOC_H

#include <sstream>
#include <algorithm>
#include <string>
#include "fundamentals.h"

std::vector <SkeletonPiece> transformDoc(const std::string &code);
void insertToSkeleton(std::vector <SkeletonPiece> &skeleton, const std::string &word, int lineNo);

#endif // TRANSFORMDOC_H
