#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "headers/image.h"
#include "headers/displayarea.h"

#include<QWidget>
#include<QAction>
#include<QMessageBox>
#include<QFileDialog>
#include<QtAlgorithms>
#include<QLabel>
#include<algorithm>
#include<QTimer>
#include<QGraphicsScene>
#include<QGraphicsPixmapItem>
#include<QGraphicsItem>
#include<QMainWindow>
#include<QScrollArea>
#include<QDir>
#include<QDebug>
#include<QScrollBar>
#include<QVector>
#include<QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    //polja
    int angle = 0;

    Ui::MainWindow *ui;
    DisplayArea* display;
    QImage image_copy; //ovo za sada jedino resenje za kompoziciju efekata

private slots:
    void on_pbNextEdit_clicked();
    void on_pbNextFinish_clicked();
    void on_pbBackStart_clicked();
    void on_pbBackEdit_clicked();
    void on_pbImport_clicked();
    void on_pbImportMultiple_clicked();
    void on_toolButton_5_clicked();
    void on_toolButton_6_clicked();
    void on_hsScale_2_sliderMoved(int position);
    void on_hsHorizontal_2_sliderMoved(int position);
    void on_hsVertical_2_sliderMoved(int position);
    void on_pbGreyscale_clicked();
    void on_hsBrightness_sliderMoved(int position);
    void on_hsBrightness_sliderReleased();
    void on_hsContrast_sliderMoved(int position);
    void on_hsContrast_sliderReleased();
    void on_hsCorrection_sliderMoved(int position);
    void on_hsCorrection_sliderReleased();
    void on_hsSaturation_sliderMoved(int position);
    void on_hsSaturation_sliderReleased();
};

#endif // MAINWINDOW_H

