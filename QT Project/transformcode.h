#ifndef TRANSFORMCODE_H
#define TRANSFORMCODE_H

#include <QtDebug>
#include <QDesktopWidget>
#include <QMessageBox>

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>

#include "cleancode.h"

std::vector <SkeletonPiece> transformCode(const std::string &code, std::unordered_set <std::string> &ignorable,
                                                   std::unordered_set <std::string> &dataTypes,
                                                   std::unordered_set <std::string> &loops,
                                                   std::unordered_set <std::string> &conditions);
bool processDeclarations(std::stringstream &ss, std::string &word, const std::string &code, int &lineNo,
                         std::vector <SkeletonPiece> &skeleton, std::unordered_set <std::string> &ignorable,
                         std::unordered_set <std::string> &dataTypes);
bool processVariableDeclarations(std::stringstream &ss, std::string &word, const std::string &code, int &lineNo,
                                 std::vector <SkeletonPiece> &skeleton);
bool processFunctionDeclarations(std::stringstream &ss, const std::string &code, int &lineNo);
bool processStatement(std::stringstream &ss, std::string &word, const std::string &code, int &lineNo,
                      std::vector <SkeletonPiece> &skeleton, bool isConditionOrLoop);
bool processFunctionCall(std::stringstream &ss, std::string &word, const std::string &code, int &lineNo,
                         std::vector <SkeletonPiece> &skeleton);
bool processConditions(std::stringstream &ss, std::string &word, const std::string &code, int &lineNo,
                       std::vector <SkeletonPiece> &skeleton);
bool processLoops(std::stringstream &ss, std::string &word, const std::string &code, int &lineNo,
                  std::vector <SkeletonPiece> &skeleton);
bool skipTypedefs(std::stringstream &ss, std::string &word, const std::string &code, int &lineNo);
void insertToSkeleton(std::vector <SkeletonPiece> &skeleton, const std::string &word, int lineNo);
bool getNextWord(std::stringstream &ss, std::string &word, const std::string &code, int &lineNo, int count = 1);
void detectLineNumber(std::stringstream &ss, const std::string &code, int &lineNo);
size_t findFirstChar(const std::string &code, size_t pos);
bool isNumber(const std::string &str);
bool isMathSign(char ch);
void initializeSets(std::unordered_set <std::string> &ignorable,
                    std::unordered_set <std::string> &dataTypes,
                    std::unordered_set <std::string> &loops,
                    std::unordered_set <std::string> &conditions);
void resetDataTypes(std::unordered_set <std::string> &dataTypes);


#endif
