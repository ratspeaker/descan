#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void DisplayImage(QString& fileName);




private:
    Ui::MainWindow *ui;

private slots:
    void on_pbNextEdit_clicked();
    void on_pbNextFinish_clicked();
    void on_pbBack1_clicked();
    void on_pbImport_clicked();
};

#endif // MAINWINDOW_H

