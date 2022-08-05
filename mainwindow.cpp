#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QProcess>
#include <QTimer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->progressBar->setHidden(true);

    model = new QStringListModel(this);

    QStringList List;
    model->setStringList(List);
    ui->visioFiles->setModel(model);

}

MainWindow::~MainWindow()
{
    delete model;
    delete ui;
}


void MainWindow::on_actionSelect_Visio_Files_triggered()
{
    QStringList files = QFileDialog::getOpenFileNames(
                this,
                "Select Visio Files",
                "",
                "Visio (*.vsdx *.vsd)");

    int currRow = model->rowCount();
    model->insertRows(currRow, files.count());

    for (auto &file : files) {
        QModelIndex index = model->index(currRow);
        ui->visioFiles->setCurrentIndex(index);
        model->setData(index, file);
        currRow++;
    }

}


void MainWindow::on_actionSave_triggered()
{
    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Vs2PDF", "Need at least 1 visio file to export to PDF");
        return;
    }

    ui->progressBar->setRange(0,model->rowCount());
    ui->progressBar->setValue(0);
    ui->progressBar->setHidden(false);

    for (auto &file : model->stringList()) {
        QString pdfFile = file;
        pdfFile.replace(".vsdx", ".pdf");
        pdfFile.replace(".vsd", ".pdf");


        QString exe_path = qApp->applicationDirPath() + QDir::separator() + "OfficeToPDF.exe";
        QStringList arguments;
        arguments << "/print" << "/hidden" << file << pdfFile;
        QProcess process;
        process.start(exe_path, arguments);
        process.waitForFinished();
        ui->progressBar->setValue(ui->progressBar->value() + 1);
        /*auto test = process.exitCode();
        auto test2 = process.exitStatus();
        int breakpoint = 0;*/
    }
    QTimer::singleShot(10000, [this]{ui->progressBar->setHidden(true);});
}


void MainWindow::on_removeButton_clicked()
{
    model->removeRows(ui->visioFiles->currentIndex().row(), 1);
}


void MainWindow::on_upButton_clicked()
{
    auto index = ui->visioFiles->currentIndex();
    auto data = index.data();
    auto row = index.row();

    if (model->insertRow(row-1)) {
        auto pre_idx = model->index(row-1);
        model->removeRow(row+1);
        model->setData(pre_idx, data, Qt::EditRole);
        ui->visioFiles->setCurrentIndex(pre_idx);
    }
}


void MainWindow::on_downButton_clicked()
{
    auto index = ui->visioFiles->currentIndex();
    auto data = index.data();
    auto row = index.row();

    if (model->insertRow(row+2)) {
        auto post_idx = model->index(row+2);
        model->setData(post_idx, data, Qt::EditRole);
        ui->visioFiles->setCurrentIndex(post_idx);
        model->removeRow(row);
    }
}


void MainWindow::on_actionMerge_to_PDF_triggered()
{
    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Vs2PDF", "Need at least 1 visio file to export to PDF");
        return;
    }

    QString pdfFile = QFileDialog::getSaveFileName(this,
                                                   "Export to PDF",
                                                   "",
                                                   "PDF (*.pdf)");

    ui->progressBar->setRange(0,model->rowCount());
    ui->progressBar->setValue(0);
    ui->progressBar->setHidden(false);

    for (auto &file : model->stringList()) {
        QString exe_path = qApp->applicationDirPath() + QDir::separator() + "OfficeToPDF.exe";
        QStringList arguments;
        arguments << "/print" << "/hidden" << "/pdf_append" << file << pdfFile;
        QProcess process;
        process.start(exe_path, arguments);
        process.waitForFinished();
        ui->progressBar->setValue(ui->progressBar->value() + 1);
        /*auto test = process.exitCode();
        auto test2 = process.exitStatus();
        int breakpoint = 0;*/
    }
    QTimer::singleShot(10000, [this]{ui->progressBar->setHidden(true);});
}

