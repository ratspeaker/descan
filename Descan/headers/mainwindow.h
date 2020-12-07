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
#include<QObject>
#include<QRubberBand>
#include<QPoint>
#include<QMouseEvent>

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
    /*polja*/
    int angle = 0;

    Ui::MainWindow *ui;
    DisplayArea* display;
    QImage image_copy; /*Ovo za sada jedino resenje za kompoziciju efekata.*/

    bool cropPressed = false;

    /*Oblast koja će biti isečena.*/
    QRubberBand* rubberBand = nullptr;
    bool rubberBandCreated = false;

    /*startPoint očitava vrednosti miša kada se desio događaj pressed, a endPoint kad se desi released.*/
    QPoint startPoint;
    QPoint endPoint;

private slots:
    void on_pbNextEdit_clicked();
    void on_pbNextFinish_clicked();
    void on_pbBackStart_clicked();
    void on_pbBackEdit_clicked();
    void on_pbImport_clicked();
    void on_pbImportMultiple_clicked();
    void on_hsScale_2_sliderMoved(int position);
    void on_hsScale_2_sliderReleased();
    void on_hsHorizontal_2_sliderMoved(int position);
    void on_hsHorizontal_2_sliderReleased();
    void on_hsVertical_2_sliderMoved(int position);
    void on_hsVertical_2_sliderReleased();

    void on_pbGreyscale_clicked();
    void on_hsBrightness_sliderMoved(int position);
    void on_hsBrightness_sliderReleased();
    void on_hsContrast_sliderMoved(int position);
    void on_hsContrast_sliderReleased();
    void on_hsCorrection_sliderMoved(int position);
    void on_hsCorrection_sliderReleased();
    void on_hsSaturation_sliderMoved(int position);
    void on_hsSaturation_sliderReleased();

    void enableUndo();

    void on_toolButton_clicked();
    void on_toolButton_2_clicked();
    void on_toolButton_5_clicked();

    bool eventFilter(QObject* watched, QEvent* event);

    void on_toolButton_7_clicked();

    void on_toolButton_8_clicked();

    void on_toolButton_4_clicked();

    void on_toolButton_3_clicked();

signals:
    void enableUndoSignal();
};

#endif // MAINWINDOW_H

