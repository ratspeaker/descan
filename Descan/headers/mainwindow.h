#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"image.hpp"
#include<QWidget>
#include<QAction>
#include<QMessageBox>
#include<QMenu>
#include<QMenuBar>
#include<QFileDialog>
#include<QtAlgorithms>
#include<QLabel>
#include<algorithm>
#include<QTimer>
#include<QGraphicsScene>
#include<QGraphicsPixmapItem>
#include<QGraphicsItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    cv::Mat Rotate_clicked(cv::Mat img, int i);

public:
    //polja
    int angle = 0;

private:
    Ui::MainWindow *ui;
    Image *m_img;

private slots:
    void on_pbNextEdit_clicked();
    void on_pbNextFinish_clicked();
    void on_pbBack1_clicked();
    void on_pbImport_clicked();
    void on_pbImportMultiple_clicked();
    void on_hsBrightness_3_sliderMoved(int position);
    void on_toolButton_5_clicked();
    void on_toolButton_6_clicked();
};

#endif // MAINWINDOW_H

