#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "resultwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    setTweaks();
    initializeComboBox();
    initializeExtensions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushDir_clicked()
{
    if (ui->radio_C->isChecked())
        currExtension = "C";
    else if (ui->radio_TXT->isChecked())
        currExtension = "TXT";
    else if (ui->radio_PDF->isChecked())
        currExtension = "PDF";
    else
        currExtension = ui->comboBox->currentText().toStdString();

    // getting source directory from the user
    QString dir = QFileDialog::getExistingDirectory(this, tr("Klasör Seç"), "/home",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir.size() > 0)
        ui->pushDir->setText(dir);

    fillTextBrowser(dir); // filling the text box to inform user that which files found
}

// A function which fills selected file names to a text box.
void MainWindow::fillTextBrowser(QString dir)
{
    codeFileList.clear();
    ui->textBrowser->setText("");

    QDir directory(dir);
    QStringList codeFiles;

    if (ui->checkFolderMode->isChecked()) // looking for folders
        codeFiles = directory.entryList(QDir::Dirs);
    else                                  // looking for selected file type
        codeFiles = directory.entryList(extensions[currExtension], QDir::Files);

    for (auto filename: codeFiles)
    {
        qDebug() << filename;

        if (filename[0] != '.') // hidden files will be ignored
        {
            codeFileList.push_back("'" + dir.toStdString() + "/" + filename.toStdString() + "'");
            ui->textBrowser->append(filename);
        }
    }
}

void MainWindow::on_pushStart_clicked()
{
    // it must be some files to process calculations
    if (codeFileList.size() > 0)
    {
        ui->pushStart->setText("İşleniyor...");
        ui->pushStart->setStyleSheet("QPushButton { color: white; background: #9b59b6; }");
        ui->pushStart->repaint();

        k = ui->slider_k->value(); // getting specified fingerprint length from user
        w = ui->slider_w->value(); // getting specified window size from user

        // opening result window
        ResultWindow *window = new ResultWindow(codeFileList, extensions[currExtension],
                                                ui->checkFolderMode->isChecked(), k, w);
        window->show();

        ui->pushStart->setText("Hesapla");
        ui->pushStart->setStyleSheet("QPushButton { color: white; background: rgb(245, 121, 0); }");
        ui->pushStart->repaint();
    }
}

// A function which initialize tweaks.
void MainWindow::setTweaks()
{
    ui->text_k->setReadOnly(true);
    ui->text_w->setReadOnly(true);

    ui->text_k->setText(QString::number(ui->slider_k->value()));
    ui->text_w->setText(QString::number(ui->slider_w->value()));

    ui->label_k->setEnabled(false);
    ui->label_w->setEnabled(false);
    ui->text_k->setEnabled(false);
    ui->text_w->setEnabled(false);
    ui->slider_k->setEnabled(false);
    ui->slider_w->setEnabled(false);
}

// A function which initialize combo box.
void MainWindow::initializeComboBox()
{
    ui->comboBox->addItem("Python");
    ui->comboBox->addItem("C++");
    ui->comboBox->addItem("C#");
    ui->comboBox->addItem("Java");
    ui->comboBox->addItem("Kotlin");
    ui->comboBox->addItem("JavaScript");
    ui->comboBox->addItem("Pascal");
    ui->comboBox->addItem("Go");
    ui->comboBox->addItem("Assembly");
    ui->comboBox->addItem("MatLAB/Octave");
    ui->comboBox->addItem("Perl");
    ui->comboBox->addItem("Haskell");
    ui->comboBox->addItem("Lisp");
    ui->comboBox->addItem("FORTRAN");
    ui->comboBox->addItem("PHP");
    ui->comboBox->addItem("HTML");
    ui->comboBox->addItem("CSS");
    ui->comboBox->addItem("XML");
}

// A function which initialize extensions.
void MainWindow::initializeExtensions()
{
    currExtension = "C";

    extensions = { {"C", {"*.c", "*.h"}},
                   {"TXT", {"*.txt"}},
                   {"PDF", {"*.pdf"}},
                   {"Python", {"*.py"}},
                   {"C++", {"*.cpp", "*.hpp", "*.c", "*.h"}},
                   {"C#", {"*.cs"}},
                   {"Java", {"*.java"}},
                   {"Kotlin", {"*.kt"}},
                   {"JavaScript", {"*.js"}},
                   {"Pascal", {"*.pas"}},
                   {"Go", {"*.go"}},
                   {"Assembly", {"*.asm", "*.s", "*.masm", "*.nasm"}},
                   {"MatLAB/Octave", {"*.m", "*.mat"}},
                   {"Perl", {"*.pl"}},
                   {"Haskell", {"*.hs", "*.lhs"}},
                   {"Lisp", {"*.lisp", "*.cl"}},
                   {"FORTRAN", {"*.f", "*.for", "*.f90", "*.f95", "*.f03", "*.f15"}},
                   {"PHP", {"*.php"}},
                   {"HTML", {"*.html"}},
                   {"CSS", {"*.css"}},
                   {"XML", {"*.xml"}} };
}

void MainWindow::on_checkTweaks_stateChanged(int arg1)
{
    ui->label_k->setEnabled(arg1);
    ui->label_w->setEnabled(arg1);
    ui->text_k->setEnabled(arg1);
    ui->text_w->setEnabled(arg1);
    ui->slider_k->setEnabled(arg1);
    ui->slider_w->setEnabled(arg1);

    if (arg1 == 0)
    {
        ui->slider_k->setValue(40);
        ui->slider_w->setValue(1);
        ui->text_k->setText(QString::number(ui->slider_k->value()));
        ui->text_w->setText(QString::number(ui->slider_w->value()));
    }
}

void MainWindow::on_slider_k_valueChanged(int value)
{
    ui->text_k->setText(QString::number(value));
}

void MainWindow::on_slider_w_valueChanged(int value)
{
    ui->text_w->setText(QString::number(value));
}

void MainWindow::on_radio_Other_toggled(bool checked)
{
    ui->comboBox->setEnabled(checked);
}

void MainWindow::on_actionDedektif_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_actionRehber_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionHakk_nda_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
}
