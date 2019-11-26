#include "addtypedefs.h"

// A function which processes type definitions of a C source code.
bool processTypedefs(std::unordered_set <std::string> &dataTypes, std::string &pureCode)
{
    std::stringstream ss(pureCode);
    std::string word;

    while (!ss.eof())
    {
        if (word == "typedef")
        {
            if (!getNextWord(ss, word))
                return false;

            if (word == "struct" || word == "union")
            {
                if (!addStructureType(dataTypes, ss, word))
                    return false;
            }
            else // a type name abbreviation, like "typedef unsigned long int uli;"
            {
                if (!addType(dataTypes, ss, word))
                    return false;
            }
        }
        else if (word == "#define")
        {
            if (!addDefinition(dataTypes, ss, word))
                return false;
        }

        if (!getNextWord(ss, word))
            return false;
    }

    return true;
}

// A function which adds a struct/union data type.
bool addStructureType(std::unordered_set<std::string> &dataTypes, std::stringstream &ss, std::string &word)
{
    while (word != "}")
        if (!getNextWord(ss, word))
            return false;

    if (!getNextWord(ss, word))
        return false;

    if (word != ";")
    {
        dataTypes.insert(word);

        if (!getNextWord(ss, word))
            return false;
    }

    return true;
}

// A function which adds a type name abbreviation, like "typedef unsigned long int newTypeName"
bool addType(std::unordered_set<std::string> &dataTypes, std::stringstream &ss, std::string &word)
{
    std::string prev;

    while (word != ";")
    {
        prev = word;

        if (!getNextWord(ss, word))
            return false;
    }

    dataTypes.insert(prev);

    return true;
}

// A function which adds type name abbreviation, like "#define newTypeName long int"
bool addDefinition(std::unordered_set<std::string> &dataTypes, std::stringstream &ss, std::string &word)
{
    std::string line, name;
    getline(ss, line);
    std::stringstream ssLine(line);

    if (!getNextWord(ssLine, name))
        return false;

    while (!ssLine.eof())
        if (!getNextWord(ssLine, word))
            return false;

    if (dataTypes.find(word) != dataTypes.end())
        dataTypes.insert(name);

    return true;
}

// A function which gets next word from the string stream.
bool getNextWord(std::stringstream &ss, std::string &word, int count)
{
    while (count--)
    {
        if (ss.eof())
            return false;

        ss >> word;
    }

    return true;
}
