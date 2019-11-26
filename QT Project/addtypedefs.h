#ifndef ADDTYPEDEFS_H
#define ADDTYPEDEFS_H

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>

#include <QDebug>

bool processTypedefs(std::unordered_set <std::string> &dataTypes, std::string &pureCode);
bool addStructureType(std::unordered_set <std::string> &dataTypes, std::stringstream &ss, std::string &word);
bool addType(std::unordered_set <std::string> &dataTypes, std::stringstream &ss, std::string &word);
bool addDefinition(std::unordered_set <std::string> &dataTypes, std::stringstream &ss, std::string &word);
bool getNextWord(std::stringstream &ss, std::string &word, int count = 1);

#endif // ADDTYPEDEFS_H
