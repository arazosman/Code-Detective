#include "transformdoc.h"

// A function which extracts the code skeleton from its full code for non-C files.
std::vector <SkeletonPiece> transformDoc(const std::string &code)
{
    std::vector <SkeletonPiece> skeleton;
    std::stringstream ss(code);
    std::string line;

    int lineNo = 1;

    while (!ss.eof())
    {
        getline(ss, line);
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end()); // removing spaces from the line
        insertToSkeleton(skeleton, line, lineNo++);
    }

    return skeleton;
}
