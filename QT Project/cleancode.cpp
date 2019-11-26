#include "cleancode.h"

// A function which removes comments from a C/C++/Java/C# file.
// Algorithm used in: https://github.com/waltersharpWEI/CommentEliminator
std::string removeComments(const std::string &path)
{
    std::string result;

    QFile file(path.c_str());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "file could not be opened";
        return result;
    }

    QTextStream stream(&file);

    // prev is the last character readed
    // curr is the current character from input file
    char prev = 0, curr = 0;

    int status = 0; // 0 for code, 1 for string, 2 for one line comment, 3 for multiline comment
    int es = 0;

    while (!stream.atEnd())
    {
        stream >> curr;

        switch (status)
        {
            // if cursor is in the normal code area
            case 0:
            {
                if (prev == '/')
                {
                    if (curr == '/')
                        status = 2;
                    else if (curr == '*')
                        status = 3;
                    else
                    {
                        result += prev;
                        result += curr;
                    }
                }
                else if (curr != '/')
                {
                    result += curr;

                    if (prev != '\\' && prev != '\'' && curr == '"')
                        status = 1;
                }

                break;
            }
            // if cursor is in a string
            case 1:
            {
                // if there is a single \ to continue the line, just put it into Buffer,
                // together with the \r or \n in Now
                if (prev == '\\')
                {
                    if (es != 1)
                    {
                        result += prev;
                        result += curr;
                        es = 0;
                    }
                    else
                    {
                        result += curr;

                        if (curr == '"')
                            status = 0;

                        es = 0;
                    }
                    if (curr == '\\')
                        es = 1;
                // otherwise just add Now to Buffer
                }
                else if (curr != '\\')
                {
                    result += curr;

                    if (curr == '"')
                        status = 0;
                }

                break;
            }
            // if cursor is in a single line comment
            case 2:
            {
                // if there is no single \ to continue the line, then stop single lien comment, add /n into Buffer
                if (prev != '\\' && (curr == '\n' || curr == '\r'))
                {
                    result += curr;
                    curr = 0;
                    status = 0;
                }

                break;
            }
            // if cursor is in a multiple line comment
            case 3:
            {
                if (prev == '*' && curr == '/')
                {
                    status = 0;
                    curr = 0;
                }

                break;
            }
            default:
            {
                break;
            }
        }

        // update the prev character, move forward to next character
        prev = curr;
    }

    return result;
}

// A function which adjusts spaces of the source code.
std::string adjustSpaces(const std::string &fullCode)
{
    std::stringstream ss(fullCode);
    std::string line, newCode, result;

    while (getline(ss, line))
        newCode += formatLine(line);

    return newCode;
}

// A function which formats the given line of the code.
std::string formatLine(const std::string &line)
{
    std::string formattedLine;
    size_t j = 0;

    while (j < line.size())
    {
        if (j+1 < line.size() && ((line[j] == '\'' && line[j+1] == '\'') || (line[j] == '"' && line[j+1] == '"')))
        {
            formattedLine.push_back(' ');
            formattedLine.push_back(line[j]);
            formattedLine.push_back(line[j+1]);
            formattedLine.push_back(' ');
            ++j;
        }
        else if (line[j] != '_' && line[j] != '#' && ispunct(line[j]))
        {
            formattedLine.push_back(' ');
            formattedLine.push_back(line[j]);
            formattedLine.push_back(' ');
        }
        else
            formattedLine.push_back(line[j]);

        ++j;
    }

    return formattedLine + "\n";
}

// A function which removes contents of quotes.
void removeQuotes(std::string &code)
{
    size_t pos_beg = code.find('"'), pos_end;

    while (pos_beg != std::string::npos)
    {
        pos_end = code.find('"', pos_beg+1);

        if (pos_end != std::string::npos)
            code.erase(pos_beg+1, pos_end-pos_beg-1);

        pos_beg = code.find('"', pos_beg+2);
    }

    pos_beg = code.find('\'');

    while (pos_beg != std::string::npos)
    {
        pos_end = code.find('\'', pos_beg+1);

        if (pos_end != std::string::npos)
            code.erase(pos_beg+1, pos_end-pos_beg-1);

        pos_beg = code.find('\'', pos_beg+2);
    }
}
