#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSelect_Visio_Files_triggered();

    void on_actionSave_triggered();


    void on_removeButton_clicked();

    void on_upButton_clicked();

    void on_downButton_clicked();

    void on_actionMerge_to_PDF_triggered();

private:
    Ui::MainWindow *ui;
    QStringListModel *model;
};
#endif // MAINWINDOW_H
