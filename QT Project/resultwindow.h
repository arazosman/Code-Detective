#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QTableWidget>
#include <QWidget>
#include <QDesktopWidget>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QTextBrowser>

#include "processdocuments.h"
#include "detailwindow.h"

namespace Ui {
class ResultWindow;
}

struct CodePair
{
    int x, y, sameFingerprints, sim1, sim2;
    CodePair(int x, int y, int sameFingerprints, int sim1, int sim2): x(x), y(y),
             sameFingerprints(sameFingerprints), sim1(sim1), sim2(sim2) {}
};

class ResultWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ResultWindow(QWidget *parent = nullptr);

    explicit ResultWindow(std::vector <std::string> &codeFileList, QStringList &extensions,
                          bool isFolderMode, int k, int w);

    ~ResultWindow();

private slots:
    void on_tableCodes_cellDoubleClicked(int row, int column);

    void on_searchButton_clicked();

    void on_searchValue_returnPressed();

private:
    Ui::ResultWindow *ui;
    void resizeTable();
    void fillTable(std::vector <Code> &codes, std::vector <CodePair> &similarities);
    std::vector <CodePair> calculateSimilarities(std::vector <Code> &codes);
    std::string getInvalidFiles(const std::vector<std::string> &invalidCodes);
    void searchRows(const QString &value);

    std::vector <Code> codes;
    std::vector <std::string> codeFileList;
    QStringList extension;
    bool isFolderMode;
    int k, w;
};

#endif // RESULTWINDOW_H
