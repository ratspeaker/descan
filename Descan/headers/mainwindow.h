#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "headers/image.h"
#include "headers/displayarea.h"
#include "headers/pdfhandler.h"
#include <QWidget>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QtAlgorithms>
#include <QLabel>
#include <algorithm>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QMainWindow>
#include <QScrollArea>
#include <QDir>
#include <QDebug>
#include <QScrollBar>
#include <QVector>
#include <QPixmap>
#include <QObject>
#include <QRubberBand>
#include <QPoint>
#include <QMouseEvent>
#include <QInputDialog>
#include <vector>
#include <QPdfWriter>
#include <QPainter>

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
    Ui::MainWindow *ui;
    DisplayArea *display;
    QImage image_copy; //ovo za sada jedino resenje za kompoziciju efekata

    bool cropPressed = false;
    int angle = 0;

    //oblast koja ce biti isecena
    QRubberBand *rubberBand = nullptr;
    bool rubberBandCreated = false;

    //startPoint ocitava vrednosti misa kada se desi događaj pressed, a endPoint kada se desi released
    QPoint startPoint;
    QPoint endPoint;

    //current slider position
    int poss = 0;

private slots:
    void showPreviousPage();
    void showNextPage();

    void on_pbImport_clicked();
    void on_pbImportMultiple_clicked();

    //slotovi za hendlovanje dimenzija slike
    void on_hsScale_sliderMoved(int position);
    void on_hsScale_sliderReleased();
    void on_hsHorizontal_sliderMoved(int position);
    void on_hsHorizontal_sliderReleased();
    void on_hsVertical_sliderMoved(int position);
    void on_hsVertical_sliderReleased();

    //slotovi za hendlovanje boja slike
    void on_pbGreyscale_clicked();
    void on_hsBrightness_sliderMoved(int position);
    void on_hsBrightness_sliderReleased();
    void on_hsContrast_sliderMoved(int position);
    void on_hsContrast_sliderReleased();
    void on_hsCorrection_sliderMoved(int position);
    void on_hsCorrection_sliderReleased();
    void on_hsSaturation_sliderMoved(int position);
    void on_hsSaturation_sliderReleased();

    //slotovi i pomocne funkcije za hendlovanje undo i redo akcija
    void enableUndo();
    void on_tbUndo_clicked();
    void on_tbRedo_clicked();

    //slotovi za uvecan/umanjen prikaz slike
    void on_tbZoomIn_clicked();
    void on_tbZoomOut_clicked();
    void on_tbFit_clicked();

    //slot za kropovanje slike
    void on_tbCrop_clicked();

    //slotovi za rotaciju
    void on_tbRotateLeft_clicked();
    void on_tbRotateRight_clicked();

    bool eventFilter(QObject* watched, QEvent* event);

    void cleanDisplayArea();
    void moveSliders();
    void clearSliderValues();
    void resetZoomButtons();
    void resetUndoRedoButtons();

    //slotovi za prethodnu i narednu sliku
    void on_pbLeftImage_clicked();
    void on_pbRightImage_clicked();

    //slot za konvertovanje slika u pdf
    void on_pbConvert_clicked();

    void changeUndoState();
    void changeRedoState();

    //slotovi za split i merge pdf
    void on_pbSplitPdf_clicked();
    void on_pbMergePdf_clicked();

    void on_pbFinish_clicked();

    void on_pbMail_clicked();

signals:
    void enableUndoSignal();
    void changeUndoSignal();
    void changeRedoSignal();
    void moveSlidersSignal();
};

#endif // MAINWINDOW_H
