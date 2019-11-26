#include "transformcode.h"

// A function which extracts C code skeleton from its pure code.
std::vector <SkeletonPiece> transformCode(const std::string &code, std::unordered_set <std::string> &ignorable,
                                                   std::unordered_set <std::string> &dataTypes,
                                                   std::unordered_set <std::string> &loops,
                                                   std::unordered_set <std::string> &conditions)
{
    std::stringstream ss(code); // stringstreams used to tokenize strings
    std::vector <SkeletonPiece> skeleton, err;
    std::string word;
    int lineNo = 1;

    if (!getNextWord(ss, word, code, lineNo)) // if the file is empty
        return err;

    while (!ss.eof()) // while there is a token
    {
        // if there is a preprocessor, skip it
        if (word[0] == '#')
        {
            getline(ss, word);
            detectLineNumber(ss, code, ++lineNo);

            if (!getNextWord(ss, word, code, lineNo))
                return err;
        }
        // if an ignorable key detected, then skip it
        else if (ignorable.find(word) != ignorable.end())
        {
            if (!getNextWord(ss, word, code, lineNo))
                return err;

            // if a semi-colon detected after an ignorable key, skip it too
            if (word == ";")
            {
                if (!getNextWord(ss, word, code, lineNo))
                    return err;
            }
        }
        // if a data type (int, double, char ...) detected:
        else if (dataTypes.find(word) != dataTypes.end())
        {
            if (!processDeclarations(ss, word, code, lineNo, skeleton, ignorable, dataTypes))
                return err;
        }
        // if a conditional statement (if, else if, else, switch-case) deteceted:
        else if (conditions.find(word) != conditions.end())
        {
            if (!processConditions(ss, word, code, lineNo, skeleton))
                return err;
        }
        // if a loop (for, while, do) detected:
        else if (loops.find(word) != loops.end())
        {
            if (!processLoops(ss, word, code, lineNo, skeleton))
                return err;
        }
        // if a type definition detected, skip it (type definitions handled
        // before extracting the skeleton)
        else if (word == "typedef")
        {
            if (!skipTypedefs(ss, word, code, lineNo))
                return err;
        }
        // skip goto statments
        else if (word == "goto")
        {
            if (!getNextWord(ss, word, code, lineNo, 3))
                return err;
        }
        // process return key
        else if (word == "return")
        {
            insertToSkeleton(skeleton, "_ret_", lineNo);

            if (!getNextWord(ss, word, code, lineNo))
                return err;
        }
        // for others:
        else
        {
            if (!processStatement(ss, word, code, lineNo, skeleton, false) || !getNextWord(ss, word, code, lineNo))
                return err;
        }
    }

    return skeleton;
}

// A function which processes variable and function declarations.
bool processDeclarations(std::stringstream &ss, std::string &word, const std::string &code, int &lineNo,
                         std::vector <SkeletonPiece> &skeleton, std::unordered_set <std::string> &ignorable,
                         std::unordered_set <std::string> &dataTypes)
{
    if (word == "struct" || word == "union" || word == "enum")
    {
        if (!getNextWord(ss, word, code, lineNo))
            return false;
    }

    std::string name;

    if (!getNextWord(ss, name, code, lineNo))
        return false; // getting the name of the variable/function

    if (name == "{") // a struct/union/enum type declaration
    {
        while (word != "}")
            if (!getNextWord(ss, word, code, lineNo))
                return false;

        while (word != ";")
            if (!getNextWord(ss, word, code, lineNo))
                return false;
    }
    else
    {
        std::string prefix;

        // we ignore additional part of data types, like "long" int x, "const static signed" long x...
        // also if there is a pointer or a reference, we ignore it
        while (ignorable.find(name) != ignorable.end() || dataTypes.find(name) != dataTypes.end() ||
               name[0] == '*' || name[0] == '&')
        {
            if (name[0] == '*' || name[0] == '&')
                prefix += name;

            if (!getNextWord(ss, name, code, lineNo))
                return false;
        }

        if (!getNextWord(ss, word, code, lineNo))
            return false; // getting the punctuation after the name

        // if the following punctuation is a comma or a equals sign,
        // it means that we found a variable assignment
        if (word == "," || word == "=" || word == "[")
        {
            insertToSkeleton(skeleton, prefix, lineNo);

            if (!processVariableDeclarations(ss, word, code, lineNo, skeleton))
                return false;
        }
        else if (word == "(") //  a function declaration
        {
            if (!processFunctionDeclarations(ss, code, lineNo))
                return false;
        }
    }

    if (!getNextWord(ss, word, code, lineNo))
        return false;

    return true;
}

// A function which processes variable declarations.
bool processVariableDeclarations(std::stringstream &ss, std::string &word, const std::string &code, int &lineNo,
                                 std::vector <SkeletonPiece> &skeleton)
{
    while (word != ";")
    {
        if (word == ",")
        {
            if (skeleton.size() > 0 && (skeleton.back().ch == '_' || skeleton.back().ch == ')'))
                insertToSkeleton(skeleton, ";", 1);

            if (!getNextWord(ss, word, code, lineNo))
                return false;

            while (word[0] == '*' || word[0] == '&')
            {
                insertToSkeleton(skeleton, word, lineNo);

                if (!getNextWord(ss, word, code, lineNo))
                    return false;
            }

            if (!getNextWord(ss, word, code, lineNo))
                return false;
        }
        else if (word == "=")
        {
            if (!getNextWord(ss, word, code, lineNo))
                return false;

            if (word == "{")
            {
                int numOfOpenCurlyBraces = 1;
                insertToSkeleton(skeleton, "_var_=_var_", lineNo);

                while (numOfOpenCurlyBraces > 0)
                {
                    if (!getNextWord(ss, word, code, lineNo))
                        return false;

                    if (word == "{")
                        ++numOfOpenCurlyBraces;
                    else if (word == "}")
                        --numOfOpenCurlyBraces;
                }

                insertToSkeleton(skeleton, ";", lineNo);

                if (!getNextWord(ss, word, code, lineNo))
                    return false;
            }
            else
            {
                insertToSkeleton(skeleton, "_var_=", lineNo);

                if (!processStatement(ss, word, code, lineNo, skeleton, false))
                    return false;
            }
        }
        else if (word == "[")
        {
            while (word != "]")
                if (!getNextWord(ss, word, code, lineNo))
                    return false;

            if (!getNextWord(ss, word, code, lineNo))
                return false;
        }
    }

    return true;
}

// A function which processes function declarations.
bool processFunctionDeclarations(std::stringstream &ss, const std::string &code, int &lineNo)
{
    int numOfOpenParantheses = 1;
    std::string word;

    if (!getNextWord(ss, word, code, lineNo))
        return false;

    while (numOfOpenParantheses > 0)
    {
        if (word == "(")
            ++numOfOpenParantheses;
        else if (word == ")")
            --numOfOpenParantheses;

        if (!getNextWord(ss, word, code, lineNo))
            return false;
    }

    return true;
}

// A function which processes statements.
bool processStatement(std::stringstream &ss, std::string &word, const std::string &code, int &lineNo,
                      std::vector <SkeletonPiece> &skeleton, bool isConditionOrLoop)
{
    int numOfOpenParantheses = 1;

    while (word != "," && word != ";" && numOfOpenParantheses > 0)
    {
        if (isConditionOrLoop)
        {
            if (word == "(")
                ++numOfOpenParantheses;
            else if (word == ")")
                --numOfOpenParantheses;
        }

        if (word == "struct" || word == "union")
        {
            if (!getNextWord(ss, word, code, lineNo))
                return false;
        }
        else if (isMathSign(word[0]) || word == "(" || word == ")" || word == "&" || word == "|"
            || word == "!" || word == "." || word == ":" || word == "?" || word == "->")
        {
            insertToSkeleton(skeleton, word, lineNo);

            if (!getNextWord(ss, word, code, lineNo))
                return false;
        }
        else if (word == "[")
        {
            int numOfOpenParantheses = 1;

            while (numOfOpenParantheses > 0)
            {
                if (!getNextWord(ss, word, code, lineNo))
                    return false;

                if (word == "[")
                    ++numOfOpenParantheses;
                else if (word == "]")
                    --numOfOpenParantheses;
            }

            if (!getNextWord(ss, word, code, lineNo))
                return false;
        }
        else
        {
            if (!getNextWord(ss, word, code, lineNo))
                return false;

            if (word == "(")
            {
                if (!processFunctionCall(ss, word, code, lineNo, skeleton) || !getNextWord(ss, word, code, lineNo))
                    return false;
            }
            else
                insertToSkeleton(skeleton, "_var_", lineNo);
        }
    }

    if ((word == ";" || word == ",") && !isConditionOrLoop && skeleton.size() > 0 &&
                                        (skeleton.back().ch == '_' || skeleton.back().ch == ')' ||
                                         skeleton.back().ch == '+' || skeleton.back().ch == '-'))
        insertToSkeleton(skeleton, ";", lineNo);

    return true;
}
// A function which processes function calls.
bool processFunctionCall(std::stringstream &ss, std::string &word, const std::string &code, int &lineNo,
                         std::vector <SkeletonPiece> &skeleton)
{
    insertToSkeleton(skeleton, "_fnc_(", lineNo);

    if (!getNextWord(ss, word, code, lineNo))
        return false;

    if (word != ")")
    {
        int numOfOpenParantheses = 1;
        insertToSkeleton(skeleton, "_par_", lineNo);

        while (numOfOpenParantheses > 0)
        {
            if (word == "(")
                ++numOfOpenParantheses;
            else if (word == ")")
                --numOfOpenParantheses;

            if (numOfOpenParantheses == 1 && word == ",")
                insertToSkeleton(skeleton, ",_par_", lineNo);

            if (numOfOpenParantheses > 0)
            {
                if (!getNextWord(ss, word, code, lineNo))
                    return false;
            }
        }
    }

    insertToSkeleton(skeleton, ")", lineNo);

    return true;
}

// A function which processes conditional statements.
bool processConditions(std::stringstream &ss, std::string &word, const std::string &code, int &lineNo,
                       std::vector <SkeletonPiece> &skeleton)
{
    if (word == "switch")
    {
        if (!getNextWord(ss, word, code, lineNo, 2))
            return false;

        std::vector <SkeletonPiece> temp;

        if (!processStatement(ss, word, code, lineNo, temp, true) || !getNextWord(ss, word, code, lineNo))
            return false;
    }
    else if (word == "case")
    {
        insertToSkeleton(skeleton, "_cnd_(_var_==_var_)", lineNo);

        while (word != ":")
            if (!getNextWord(ss, word, code, lineNo))
                return false;

        if (!getNextWord(ss, word, code, lineNo))
            return false;
    }
    else if (word == "default")
    {
        insertToSkeleton(skeleton, "_cnd_()", lineNo);

        if (!getNextWord(ss, word, code, lineNo, 2))
            return false;
    }
    else
    {
        bool isElse = (word == "else");

        if (!getNextWord(ss, word, code, lineNo))
            return false;

        if (isElse && word != "if")
            insertToSkeleton(skeleton, "_cnd_()", lineNo);
        else
        {
            if (word == "if") // the condition is "else if"
            {
                if (!getNextWord(ss, word, code, lineNo)) // getting '(' character
                    return false;
            }

            if (!getNextWord(ss, word, code, lineNo))
                return false;

            insertToSkeleton(skeleton, "_cnd_(", lineNo);

            if (!processStatement(ss, word, code, lineNo, skeleton, true))
                return false;
        }
    }

    return true;
}

// A function which processes loops.
bool processLoops(std::stringstream &ss, std::string &word, const std::string &code, int &lineNo,
                  std::vector <SkeletonPiece> &skeleton)
{
    if (word == "for")
    {
        if (!getNextWord(ss, word, code, lineNo))
            return false;

        while (word != ";")
            if (!getNextWord(ss, word, code, lineNo))
                return false;

        if (!getNextWord(ss, word, code, lineNo))
            return false;

        insertToSkeleton(skeleton, "_lop_(", lineNo);

        if (!processStatement(ss, word, code, lineNo, skeleton, true))
            return false;

        insertToSkeleton(skeleton, ")", lineNo);

        if (!getNextWord(ss, word, code, lineNo))
            return false;

        std::vector <SkeletonPiece> temp;

        if (!processStatement(ss, word, code, lineNo, temp, true))
            return false;
    }
    else if (word == "while")
    {
        if (!getNextWord(ss, word, code, lineNo, 2))
            return false;

        insertToSkeleton(skeleton, "_lop_(", lineNo);

        if (!processStatement(ss, word, code, lineNo, skeleton, true))
            return false;
    }
    else // do while
    {
        insertToSkeleton(skeleton, "_lop_()", lineNo);

        if (!getNextWord(ss, word, code, lineNo))
            return false;
    }

    return true;
}

// A function which skips type definitions.
bool skipTypedefs(std::stringstream &ss, std::string &word, const std::string &code, int &lineNo)
{
    if (!getNextWord(ss, word, code, lineNo))
        return false;

    if (word == "struct" || word == "union")
        while (word != "}")
            if (!getNextWord(ss, word, code, lineNo))
                return false;

    while (word != ";")
        if (!getNextWord(ss, word, code, lineNo))
            return false;

    if (!getNextWord(ss, word, code, lineNo))
        return false;

    return true;
}

// A function which insert a word to the skeleton with its line number.
void insertToSkeleton(std::vector <SkeletonPiece> &skeleton, const std::string &word, int lineNo)
{
    for (const char &ch: word)
        skeleton.push_back({ch, lineNo});
}

// A function which gets next word from the string stream.
bool getNextWord(std::stringstream &ss, std::string &word, const std::string &code, int &lineNo, int count)
{
    while (count--)
    {
        if (ss.eof())
            return false;

        ss >> word;
        detectLineNumber(ss, code, lineNo);
    }

    return true;
}

// A function which increments the current line number as the number of new lines.
void detectLineNumber(std::stringstream &ss, const std::string &code, int &lineNo)
{
    size_t beg = ss.tellg();
    size_t end = findFirstChar(code, beg);

    if (end != std::string::npos)
        lineNo += std::count_if(code.begin()+beg, code.begin()+end, [] (const char &ch) { return ch == '\n'; });
}

// A function which finds the index of the first character which is not a space character.
size_t findFirstChar(const std::string &code, size_t pos)
{
    for (size_t i = pos; i < code.size(); ++i)
        if (!isspace(code[i]))
            return i;

    return std::string::npos;
}

// A function which detects if a string is a number or not.
bool isNumber(const std::string &str)
{
    if (str[0] == 'e' && str[str.size()-1] == 'e')
        return false;

    int num_of_e = 0; // the number may be in form of 1e10, 1e15, 2e3...

    for (size_t i = 0; i < str.size(); ++i)
    {
        if (!isdigit(str[i]))
        {
            if (str[i] == 'e')
            {
                ++num_of_e;

                if (num_of_e > 1)
                    return false;
            }
            else
                return false;
        }
    }

    return true;
}

// A function which detects if a character is a mathematical sign or not.
bool isMathSign(char ch)
{
    return ch == '=' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '>' || ch == '<';
}

// A function which initializes keys for C source code.
void initializeSets(std::unordered_set <std::string> &ignorable,
                    std::unordered_set <std::string> &dataTypes,
                    std::unordered_set <std::string> &loops,
                    std::unordered_set <std::string> &conditions)
{
    dataTypes = {"bool", "char", "signed", "unsigned", "short", "int", "long", "size_t",
                 "float", "double", "void", "FILE", "struct", "union", "enum", "volatile"};

    ignorable = {"const", "static", "break", "continue", "inline", "extern", "{", "}"};

    conditions = {"if", "else", "switch", "case", "default"};

    loops = {"for", "while", "do"};
}

// A function which resets data types for C source code.
void resetDataTypes(std::unordered_set <std::string> &dataTypes)
{
    dataTypes = {"bool", "char", "signed", "unsigned", "short", "int", "long", "size_t",
                 "float", "double", "void", "FILE", "struct", "union", "enum", "volatile"};
}
