#ifndef PROCESSDOCUMENTS_H
#define PROCESSDOCUMENTS_H

#include <unordered_map>
#include <fstream>
#include <QtDebug>
#include <QDir>
#include <QDesktopWidget>
#include <QDirIterator>
#include <QMessageBox>

#include "transformcode.h"
#include "transformdoc.h"
#include "addtypedefs.h"
#include "winnowing.h"

std::vector <Code> getCodes(std::vector <std::string> &codeFiles, std::vector <std::string> &invalidCodes,
                            const QStringList &extension, bool isFolderMode, int k, int w);
std::vector<Code> getCodePieces(const std::string &folderPath, const QStringList &extension);
Code *initializeCode(const std::string &path);
void getCodeC(Code &code);
std::string readFile(const std::string &path);
QString pdfToText(const std::string &path);
void setFileName(Code &code);

#endif // PROCESSDOCUMENTS_H
