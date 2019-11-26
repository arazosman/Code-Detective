#include "detailwindow.h"
#include "ui_detailwindow.h"

DetailWindow::DetailWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DetailWindow)
{
    ui->setupUi(this);
}

DetailWindow::DetailWindow(const Code &code1, const Code &code2, const QStringList &extension,
                           int matchedFP, int sim1, int sim2, int k): DetailWindow()
{
    this->code1 = code1;
    this->code2 = code2;
    this->matchedFP = matchedFP;
    this->sim1 = sim1;
    this->sim2 = sim2;
    this->k = k;
    this->extension = extension;

    fillDetails(extension);
}

DetailWindow::~DetailWindow()
{
    delete ui;
}

// A function which fills details of widgets in the window.
void DetailWindow::fillDetails(const QStringList &extension)
{
    this->setWindowTitle("Detaylar - " + QString::fromStdString(code1.fileName) + " vs " +
                         QString::fromStdString(code2.fileName));

    ui->code1_label->setText(QString::fromStdString(code1.fileName));
    ui->code2_label->setText(QString::fromStdString(code2.fileName));

    ui->simBar1->setValue(sim1);
    ui->simBar2->setValue(sim2);

    ui->code1_totalFP->setText(QString::number(code1.skeleton.size()-k+1));
    ui->code1_selectedFP->setText(QString::number(code1.numOfSelectedFingerPrints));
    ui->code1_matchedFP->setText(QString::number(matchedFP));

    ui->code2_totalFP->setText(QString::number(code2.skeleton.size()-k+1));
    ui->code2_selectedFP->setText(QString::number(code2.numOfSelectedFingerPrints));
    ui->code2_matchedFP->setText(QString::number(matchedFP));

    if (extension[0] != "*.c")
    {
        ui->code1_pushDoc->setEnabled(false);
        ui->code2_pushDoc->setEnabled(false);
        ui->code1_pushSkeleton->setHidden(true);
        ui->code2_pushSkeleton->setHidden(true);
    }

    setHighlightingLines(); // detecting which lines should be highlighted

    // highlighting the first code
    ui->code1_docText->setText(highlightCode(code1.fullCode, highlightedLines1));

    if (extension[0] == "*.c")
        ui->code1_skeletonText->setText(colorizeSkeleton(getSkeletonString(code1.skeleton)));

    // highlighting the second code
    ui->code2_docText->setText(highlightCode(code2.fullCode, highlightedLines2));

    if (extension[0] == "*.c")
        ui->code2_skeletonText->setText(colorizeSkeleton(getSkeletonString(code2.skeleton)));

    ui->code1_stack->setCurrentIndex(0);
    ui->code2_stack->setCurrentIndex(0);

    ui->code1_pushDoc->setStyleSheet("QPushButton { color: #FFF; background: rgb(92, 53, 102) }");
    ui->code2_pushDoc->setStyleSheet("QPushButton { color: #FFF; background: rgb(92, 53, 102) }");
}

// A function which detects lines which should be highlighted.
void DetailWindow::setHighlightingLines()
{
    for (const auto &p1: code1.fingerprints)
    {
        auto p2 = code2.fingerprints.find(p1.first);

        if (p2 != code2.fingerprints.end()) // if there is a match
        {
            for (auto pos = p1.second.begin(); pos != p1.second.end(); ++pos)
                for (int i = pos->beg; i <= pos->end; ++i)
                    highlightedLines1.insert(i);

            for (auto pos = p2->second.begin(); pos != p2->second.end(); ++pos)
                for (int i = pos->beg; i <= pos->end; ++i)
                    highlightedLines2.insert(i);
        }
    }
}

// A function which highlights the given code.
QString DetailWindow::highlightCode(const std::string &code, const std::set<int> &highlightedLines)
{
    std::string highlightedCode = "<pre style='white-space: pre-wrap; word-break: keep-all';>";
    std::stringstream ss(code);
    std::string line;
    int lineNo = 1;

    while (!ss.eof())
    {
        getline(ss, line);

        std::string newLine;

        for (char &ch: line)
        {
            // transforming HTML character codes
            if (ch == '<')
                newLine += "&#60;";
            else if (ch == '>')
                newLine += "&#62;";
            else if (ch == '&')
                newLine += "&amp;";
            else
                newLine += ch;
        }

        if (highlightedLines.find(lineNo++) != highlightedLines.end())
            highlightedCode += "<span style='color:#DC143C;'>" + newLine + "</span><br>";
        else
            highlightedCode += newLine + "<br>";
    }

    return QString::fromStdString(highlightedCode + "</pre>");
}

// A function which gets string from the code skeleton.
std::string DetailWindow::getSkeletonString(const std::vector<SkeletonPiece> &skeleton)
{
    std::string skeletonString;

    for (auto pos = skeleton.begin(); pos != skeleton.end(); ++pos)
        skeletonString += pos->ch;

    return skeletonString;
}

// A function which colorizes the given code skeleton.
QString DetailWindow::colorizeSkeleton(const std::string &skeletonString)
{
    std::string colorized = "<pre style='white-space: pre-wrap; word-break: keep-all';>";
    size_t i = 0;

    while (i < skeletonString.size())
    {
        if (skeletonString[i] == '_')
        {
            std::string color, word = skeletonString.substr(i, 5);

            if (word == "_fnc_")
                color = "#DC143C";
            else if (word == "_par_")
                color = "#6A5ACD";
            else if (word == "_cnd_")
                color = "#228B22";
            else if (word == "_lop_")
                color = "#00CED1";
            else if (word == "_ret_")
                color = "#2F4F4F";
            else
                color = "#F4A460";

            colorized += "<span style='color:" + color + ";'>" + word + "</span>";
            i += 5;
        }
        else
        {
            if (skeletonString[i] == '<')
                colorized += "&#60;";
            else if (skeletonString[i] == '>')
                colorized += "&#62;";
            else if (skeletonString[i] == '&')
                colorized += "&amp;";
            else
                colorized += skeletonString[i];

            ++i;
        }
    }

    return QString::fromStdString(colorized + "</pre>");
}

void DetailWindow::on_code1_pushDoc_clicked()
{
    ui->code1_stack->setCurrentIndex(0);
    ui->code1_pushDoc->setStyleSheet("QPushButton { color: #FFF; background: rgb(92, 53, 102) }");
    ui->code1_pushSkeleton->setStyleSheet("QPushButton { color: #FFF; background: rgb(10, 54, 86) }");
}

void DetailWindow::on_code1_pushSkeleton_clicked()
{
    ui->code1_stack->setCurrentIndex(1);
    ui->code1_pushDoc->setStyleSheet("QPushButton { color: #FFF; background: rgb(10, 54, 86) }");
    ui->code1_pushSkeleton->setStyleSheet("QPushButton { color: #FFF; background: rgb(92, 53, 102) }");
}

void DetailWindow::on_code2_pushDoc_clicked()
{
    ui->code2_stack->setCurrentIndex(0);
    ui->code2_pushDoc->setStyleSheet("QPushButton { color: #FFF; background: rgb(92, 53, 102) }");
    ui->code2_pushSkeleton->setStyleSheet("QPushButton { color: #FFF; background: rgb(10, 54, 86) }");
}

void DetailWindow::on_code2_pushSkeleton_clicked()
{
    ui->code2_stack->setCurrentIndex(1);
    ui->code2_pushDoc->setStyleSheet("QPushButton { color: #FFF; background: rgb(10, 54, 86) }");
    ui->code2_pushSkeleton->setStyleSheet("QPushButton { color: #FFF; background: rgb(92, 53, 102) }");
}

void DetailWindow::on_code1_pushOpen_clicked()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdString(code1.filePath.substr(1, code1.filePath.size()-2))));
}

void DetailWindow::on_code2_pushOpen_clicked()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdString(code2.filePath.substr(1, code2.filePath.size()-2))));
}
