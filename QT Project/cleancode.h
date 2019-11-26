#ifndef CLEANCODE_H
#define CLEANCODE_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <array>
#include <QString>
#include <QDebug>
#include <QFile>
#include <poppler-qt5.h>

#include <fundamentals.h>

std::string removeComments(const std::string &path);
bool skipLine(const std::string &line);
std::string adjustSpaces(const std::string &fullCode);
std::string formatLine(const std::string &line);
void removeQuotes(std::string &code);

#endif // CLEANCODE_H
