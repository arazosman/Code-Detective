#ifndef DETAILWINDOW_H
#define DETAILWINDOW_H

#include <QWidget>
#include <QDesktopServices>
#include <QDebug>

#include <algorithm>
#include <set>
#include <sstream>
#include "fundamentals.h"

namespace Ui {
class DetailWindow;
}

class DetailWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DetailWindow(QWidget *parent = nullptr);

    explicit DetailWindow(const Code &code1, const Code &code2, const QStringList &extension,
                          int matchedFP, int sim1, int sim2, int k);

    ~DetailWindow();

private slots:
    void on_code1_pushDoc_clicked();

    void on_code1_pushSkeleton_clicked();

    void on_code2_pushDoc_clicked();

    void on_code2_pushSkeleton_clicked();

    void on_code1_pushOpen_clicked();

    void on_code2_pushOpen_clicked();

private:
    Ui::DetailWindow *ui;
    void fillDetails(const QStringList &extension);
    void setHighlightingLines();
    QString highlightCode(const std::string &code, const std::set <int> &highlightedLines);
    std::string getSkeletonString(const std::vector <SkeletonPiece> &skeleton);
    QString colorizeSkeleton(const std::string &skeletonString);

    Code code1, code2;
    int matchedFP, sim1, sim2, k;
    QStringList extension;
    std::set <int> highlightedLines1, highlightedLines2;
};

#endif // DETAILWINDOW_H
