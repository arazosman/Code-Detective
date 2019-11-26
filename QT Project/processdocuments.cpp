#include "processdocuments.h"

// A function which gets and processes given codes.
// k: fingerint size
// w: window size
std::vector <Code> getCodes(std::vector <std::string> &codeFiles, std::vector <std::string> &invalidCodes,
                            const QStringList &extension, bool isFolderMode, int k, int w)
{
    std::vector <Code> codes;

    // sets for C source codes
    std::unordered_set <std::string> dataTypes, conditions, loops, ignorable;

    // getting the hash factor for given fingerprint size
    long long factor = getFactor(k);

    if (extension[0] == "*.c")
        initializeSets(ignorable, dataTypes, loops, conditions);

    for (const auto &codeFile: codeFiles)
    {
        if (extension[0] == "*.c")
            resetDataTypes(dataTypes);

        Code *code = initializeCode(codeFile);
        qDebug() << codeFile.c_str();

        std::vector <Code> codePieces;

        if (isFolderMode) // getting code pieces for folder mode
            codePieces = getCodePieces(codeFile, extension);
        else
            codePieces.push_back(*code);

        for (auto &codePiece: codePieces)
        {
            // Getting files which has C type comments:
            if (extension[0] == "*.c" || extension[0] == "*.cpp" || extension[0] == "*.cs" || extension[0] == "*.java" || extension[0] == "*.kt")
                getCodeC(codePiece);
            else if (extension[0] == "*.pdf") // getting PDF files
            {
                QString fullDoc = pdfToText(codePiece.filePath.substr(1, codePiece.filePath.size()-2));

                if (fullDoc != nullptr)
                    codePiece.fullCode = codePiece.pureCode = fullDoc.toStdString();
            }
            else    // getting other files
                codePiece.fullCode = codePiece.pureCode = readFile(codePiece.filePath.substr(1,
                                                                 codePiece.filePath.size()-2));

            // if the code is not a C file or the C code has not have problems while
            // processing type definitions then accept this code
            if (extension[0] != "*.c" || processTypedefs(dataTypes, codePiece.pureCode))
            {
                code->pureCode += codePiece.pureCode;
                code->fullCode += codePiece.fullCode;
            }
        }

        qDebug() << "pureCode";

        // extracting code skeletons:
        if (extension[0] == "*.c")
            code->skeleton = transformCode(code->pureCode, ignorable, dataTypes, loops, conditions);
        else
            code->skeleton = transformDoc(code->pureCode);

        qDebug() << "getCodeSkeleton";
        setFileName(*code);

        // size of code skeletons must be greater than the fingerprint size
        if (code->skeleton.size() <= k)
        {
            invalidCodes.push_back(code->fileName);
            qDebug() << "invalid code";
        }
        else
        {
            getFingerprints(*code, k, w, factor);
            codes.push_back(*code);
            qDebug() << "getFingerprints";
        }
    }

    return codes;
}

// A function which gets code pieces if folder mode selected.
std::vector<Code> getCodePieces(const std::string &folderPath, const QStringList &extension)
{
    std::vector <Code> codePieces;

    QDir directory(QString::fromStdString(folderPath.substr(1, folderPath.size()-2)));
    QDirIterator it(directory.path(), extension, QDir::Files, QDirIterator::Subdirectories);

    // getting all the files of specified document type in the directory and its subdirecitories
    while (it.hasNext())
    {
        QString path = it.next();

        if (QFileInfo(path).fileName()[0] != '.') // ignoring hidden files
            codePieces.push_back(*initializeCode("'" + path.toStdString() + "'"));
    }

    return codePieces;
}

// A function which initializes a code.
Code *initializeCode(const std::string &path)
{
    Code *code = new Code;
    code->filePath = path;
    return code;
}

// A function which gets and simplifies a C/C++/C#/Java/Kotlin source code.
void getCodeC(Code &code)
{
    // removing comments from the source code.
    std::string fullCode = removeComments(code.filePath.substr(1, code.filePath.size()-2));
    code.fullCode = fullCode;
    removeQuotes(fullCode); // removing quotes from the source code
    fullCode = adjustSpaces(fullCode); // adjusting spacings in the source code
    code.pureCode = fullCode;
}

// A function which reads a file.
std::string readFile(const std::string &path)
{
    QFile codeFile(QString::fromStdString(path));
    codeFile.open(QFile::ReadOnly | QFile::Text);
    std::string fullCode = codeFile.readAll().toStdString();
    codeFile.close();

    return fullCode;
}

// A function which transfroms PDF file to the text file with Poppler library.
QString pdfToText(const std::string &path)
{
    qDebug() << path.c_str();

    Poppler::Document *doc = Poppler::Document::load(QString::fromStdString(path));

    if (!doc)
       return nullptr;

    QString fullText;

    for (int i = 0; i < doc->numPages(); ++i)
        fullText += doc->page(i)->text(QRectF());

    return fullText;
}

// A function which sets name of a file from its path.
void setFileName(Code &code)
{
    auto index = code.filePath.find_last_of('/');

    if (index != std::string::npos)
        code.fileName = code.filePath.substr(index+1, code.filePath.size()-index-2);
    else
        code.fileName = code.filePath;
}
