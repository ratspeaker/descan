#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "headers/image.h"
#include "headers/displayarea.h"
#include "headers/pdfhandler.h"
#include "headers/drive.h"
#include <vector>
#include <utility>
#include <QWidget>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QtAlgorithms>
#include <QLabel>
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
    QImage imageCopy; //resenje za kompoziciju efekata

    QRubberBand *rubberBand = nullptr; //oblast koja ce biti isecena
    bool rubberBandCreated = false;
    bool cropPressed = false;

    QPoint startPoint; //ocitava vrednosti misa kada se desi dogadjaj pressed
    QPoint endPoint;   //ocitava vrednosti misa kada se desi dogadjaj released

    int currentPosition = 0; //current slider position

    QStringList filePathsPdf;
    PDFHandler *pdf;
    Drive *drive = nullptr;

    QString convertArg;

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
    void changeUndoState();
    void changeRedoState();

    //slotovi za uvecan/umanjen prikaz slike
    void on_tbZoomIn_clicked();
    void on_tbZoomOut_clicked();
    void on_tbFit_clicked();

    //slotovi za kropovanje slike
    void on_tbCrop_clicked();
    bool eventFilter(QObject* watched, QEvent* event);

    //slotovi za rotaciju
    void on_tbRotateLeft_clicked();
    void on_tbRotateRight_clicked();

    //slotovi za prethodnu i narednu sliku
    void on_pbLeftImage_clicked();
    void on_pbRightImage_clicked();

    //slot za konvertovanje slika u pdf
    void on_pbConvert_clicked();

    //slotovi za split i merge pdf
    void on_pbSplitPdf_clicked();
    void on_pbMergePdf_clicked();

    void on_pbFinish_clicked();
    void on_pbCompress_clicked();
    void on_pbMail_clicked();
    void on_pbDrive_clicked();

private:
    void cleanDisplayArea();
    void cleanFirstPageLabel();
    void moveSliders();
    void clearSliderValues();
    void resetZoomButtons();
    void resetUndoRedoButtons();
    void checkZoomButtons();
    void enableOptions();

signals:
    void enableUndoSignal();
    void changeUndoSignal();
    void changeRedoSignal();
    void moveSlidersSignal();
};

#endif // MAINWINDOW_H
