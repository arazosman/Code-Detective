#ifndef CODEWINDOW_H
#define CODEWINDOW_H

#include <QWidget>
#include <QDesktopServices>
#include <QFile>
#include <QFileInfo>
#include <QDirIterator>

#include "cleancode.h"

namespace Ui {
class CodeWindow;
}

class CodeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CodeWindow(QWidget *parent = nullptr);
    explicit CodeWindow(const std::string &code1Path, const std::string &code2Path,
                        const std::string &code1Name, const std::string &code2Name,
                        const QStringList &extension);

    ~CodeWindow();

private slots:
    void on_code1_push_clicked();

    void on_code2_push_clicked();

private:
    Ui::CodeWindow *ui;
    QString code1Path, code2Path, code1Name, code2Name;

    void displayCodes(const QStringList &extension);
    void displayCodePieces(const QStringList &extension);
};

#endif // CODEWINDOW_H
