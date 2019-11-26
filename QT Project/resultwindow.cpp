#include "resultwindow.h"
#include "ui_resultwindow.h"

ResultWindow::ResultWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
}

ResultWindow::ResultWindow(std::vector<std::string> &codeFileList, QStringList &extension,
                           bool isFolderMode, int k, int w): ResultWindow()
{
    this->codeFileList = codeFileList;
    this->k = k;
    this->w = w;
    this->isFolderMode = isFolderMode;
    this->extension = extension;

    QElapsedTimer timer; // a timer object to keep elapsed time for calculations
    timer.start();

    std::vector <std::string> invalidCodes;

    // getting and processing codes
    codes = getCodes(codeFileList, invalidCodes, extension, isFolderMode, k, w);

    std::vector <CodePair> similarities;

    if (codes.size() > 0)
    {
        // calculating code similarities
        similarities = calculateSimilarities(codes);

        // sorting the codes according to their similarities
        std::sort(similarities.begin(), similarities.end(),
                  [] (const CodePair &lhs, const CodePair &rhs) { return lhs.sim1+lhs.sim2 > rhs.sim1+rhs.sim2; });
    }

    size_t size = codeFileList.size() - invalidCodes.size();
    double sec = timer.nsecsElapsed()/1e9;

    fillTable(codes, similarities); // filling the result table

    // information message for users
    std::string message = std::to_string(size) + " adet döküman için " + std::to_string(size*(size-1)/2)
            + " tane karşılaştırma " + std::to_string(sec) + " saniyede işlendi.";

    std::string errFiles;

    if (invalidCodes.size() > 0) // if there is an invalid code
    {
        message += "\n\n" + std::to_string(invalidCodes.size()) + " adet dökümanın işlenmesi sırasında hata oluştu."
                   " (Muhtemel hata nedenleri için rehbere göz atın.)\n";

        errFiles = getInvalidFiles(invalidCodes);
    }

    QMessageBox messageBox(QMessageBox::Information, "Bilgi",
                            QObject::tr(message.c_str()),
                            QMessageBox::NoButton,
                            QDesktopWidget().screen(), Qt::FramelessWindowHint);

    messageBox.setDetailedText(errFiles.c_str());

    messageBox.exec();
}

ResultWindow::~ResultWindow()
{
    delete ui;
}

// A function which fills the result table.
void ResultWindow::fillTable(std::vector <Code> &codes, std::vector <CodePair> &similarities)
{
    resizeTable(); // resizing the table.

    // setting the header labels
    ui->tableCodes->setHorizontalHeaderLabels(QStringList()
                                              << "Sıra" << "1. Döküman" << "Benzerlik" << "2. Döküman" << "Benzerlik");

    // creating and initializing a table item instance
    QTableWidgetItem *tableItem = new QTableWidgetItem();
    tableItem->setTextAlignment(Qt::AlignCenter);
    tableItem->setFlags(tableItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);

    for (size_t i = 0; i < similarities.size(); ++i)
    {
        ui->tableCodes->insertRow(ui->tableCodes->rowCount());

        int x = similarities[i].x;
        int y = similarities[i].y;
        int sim1 = similarities[i].sim1;
        int sim2 = similarities[i].sim2;

        QTableWidgetItem *newitem = tableItem->clone(); // cloning the instance object
        newitem->setText(QString::number(i+1)); // order of the code pair
        ui->tableCodes->setItem(i, 0, newitem);

        newitem = tableItem->clone();
        newitem->setText(QString::fromStdString(codes[x].fileName)); // name of the first code
        ui->tableCodes->setItem(i, 1, newitem);

        newitem = tableItem->clone();
        newitem->setText("%" + QString::number(sim1)); // similarity rate of first code over second code
        ui->tableCodes->setItem(i, 2, newitem);

        newitem = tableItem->clone();
        newitem->setText(QString::fromStdString(codes[y].fileName)); // name of the second code
        ui->tableCodes->setItem(i, 3, newitem);

        newitem = tableItem->clone();
        newitem->setText("%" + QString::number(sim2)); // similarity rate of second code over first code
        ui->tableCodes->setItem(i, 4, newitem);

        // the table items below are not exists to display results, these are for sake
        // to pass the data to the next window when a user clicks to a code pair

        newitem = tableItem->clone();
        newitem->setText(QString::number(x)); // similarity rate of first code over second code
        ui->tableCodes->setItem(i, 5, newitem);

        newitem = tableItem->clone();
        newitem->setText(QString::number(y)); // similarity rate of second code over first code
        ui->tableCodes->setItem(i, 6, newitem);

        newitem = tableItem->clone();
        newitem->setText(QString::number(similarities[i].sameFingerprints)); // number of matched fingerprints
        ui->tableCodes->setItem(i, 7, newitem);
    }
}

// A function to resize the result table.
void ResultWindow::resizeTable()
{
    ui->tableCodes->verticalHeader()->setVisible(false);

    ui->tableCodes->setColumnWidth(0, 70);
    ui->tableCodes->setColumnWidth(1, 325);
    ui->tableCodes->setColumnWidth(2, 79);
    ui->tableCodes->setColumnWidth(3, 325);
    ui->tableCodes->setColumnWidth(4, 79);
    ui->tableCodes->setColumnWidth(5, 0);
    ui->tableCodes->setColumnWidth(6, 0);
    ui->tableCodes->setColumnWidth(7, 0);

    this->setMaximumWidth(ui->tableCodes->width());
    this->setMinimumWidth(ui->tableCodes->width());

    ui->tableCodes->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

}

// A function which calculates similarity rates of code pairs.
std::vector <CodePair> ResultWindow::calculateSimilarities(std::vector<Code> &codes)
{
    std::vector <CodePair> similarities;

    for (size_t i = 0; i < codes.size()-1; ++i)
    {
        for (size_t j = i+1; j < codes.size(); ++j)
        {
            // getting the number of matched fingerprints
            int sameFingerprints = compareCodes(codes[i], codes[j]);
            size_t less, bigger;

            if (codes[i].numOfSelectedFingerPrints < codes[j].numOfSelectedFingerPrints)
                less = j, bigger = i;
            else
                less = i, bigger = j;

            similarities.emplace_back(bigger, less, sameFingerprints, 100 * sameFingerprints /
                                      codes[bigger].numOfSelectedFingerPrints, 100 * sameFingerprints /
                                      codes[less].numOfSelectedFingerPrints);
        }
    }

    return similarities;
}

// A function which generates a text from invalid file names.
std::string ResultWindow::getInvalidFiles(const std::vector<std::string> &invalidCodes)
{
    std::string invalidFiles;

    for (const std::string &codeName: invalidCodes)
        invalidFiles += codeName + "\n";

    return invalidFiles;
}

// A function which searches a text over code names.
void ResultWindow::searchRows(const QString &value)
{
    // displaying the codes which matches with the searched text and hiding the codes which not matches
    for (int i = 0; i < ui->tableCodes->rowCount(); ++i)
        ui->tableCodes->setRowHidden(i,
        !ui->tableCodes->item(i, 1)->text().contains(value, Qt::CaseSensitivity::CaseInsensitive) &&
        !ui->tableCodes->item(i, 3)->text().contains(value, Qt::CaseSensitivity::CaseInsensitive));
}

void ResultWindow::on_tableCodes_cellDoubleClicked(int row, int column)
{
    // getting the index of first code
    int x = ui->tableCodes->item(row, 5)->text().toInt();
    // getting the index of second code
    int y = ui->tableCodes->item(row, 6)->text().toInt();
    // getting the number of matched fingerprints of code pair
    int matchedFP = ui->tableCodes->item(row, 7)->text().toInt();
    // getting the similarity rate of first code over second code:
    int sim1 = std::stoi(ui->tableCodes->item(row, 2)->text().toStdString().substr(1));
    // getting the similarity rate of second code over first code
    int sim2 = std::stoi(ui->tableCodes->item(row, 4)->text().toStdString().substr(1));

    // opening the detail window for selected code pair
    DetailWindow *window = new DetailWindow(codes[x], codes[y], extension, matchedFP, sim1, sim2, k);
    window->show();
}

void ResultWindow::on_searchButton_clicked()
{
    searchRows(ui->searchValue->text());
}

void ResultWindow::on_searchValue_returnPressed()
{
    searchRows(ui->searchValue->text());
}
