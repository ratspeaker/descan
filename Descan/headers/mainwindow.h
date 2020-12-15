#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "headers/image.h"
#include "headers/displayarea.h"

#include "../PDFNetC64/Headers/PDF/PDFNet.h"
#include "../PDFNetC64/Headers/PDF/PDFDoc.h"
#include "../PDFNetC64/Headers/PDF/PDFDocInfo.h"
#include "../PDFNetC64/Headers/SDF/SDFDoc.h"

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
#include<QInputDialog>
#include<QFileDialog>
#include<vector>

using namespace std;

using namespace pdftron;
using namespace SDF;
using namespace PDF;


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

    //current slider position
    int poss = 0;

private slots:

    void showPreviousPage();
    void showNextPage();

    void on_pbImport_clicked();
    void on_pbImportMultiple_clicked();



    /*Slotovi za hendlovanje dimenzija */
    void on_hsScale_sliderMoved(int position);
    void on_hsScale_sliderReleased();
    void on_hsHorizontal_sliderMoved(int position);
    void on_hsHorizontal_sliderReleased();
    void on_hsVertical_sliderMoved(int position);
    void on_hsVertical_sliderReleased();

    /* Slotovi za hendlovanje boja slike */
    void on_pbGreyscale_clicked();
    void on_hsBrightness_sliderMoved(int position);
    void on_hsBrightness_sliderReleased();
    void on_hsContrast_sliderMoved(int position);
    void on_hsContrast_sliderReleased();
    void on_hsCorrection_sliderMoved(int position);
    void on_hsCorrection_sliderReleased();
    void on_hsSaturation_sliderMoved(int position);
    void on_hsSaturation_sliderReleased();

    /*Slotovi i pomocne funkcije za hendlovanje undo i redo akcija */
    void enableUndo();
    void on_tbUndo_clicked();
    void on_tbRedo_clicked();

    /*Slotovi za uvecan/umanjen prikaz slike*/
    void on_tbZoomIn_clicked();
    void on_tbZoomOut_clicked();
    void on_tbFit_clicked();

    /*Slot za kropovanje slike*/
    void on_tbCrop_clicked();
    /*Slotovi za rotaciju*/
    void on_tbRotateLeft_clicked();
    void on_tbRotateRight_clicked();

    bool eventFilter(QObject* watched, QEvent* event);


    void cleanDisplayArea();
    void convertImagesIntoPdf(QString& filename);
    void moveSliders();

    void on_pbLeftImage_clicked();
    void on_pbRightImage_clicked();

    void on_pbConvert_clicked();

    void changeUndoState();
    void changeRedoState();

    void on_pbSplitPdf_clicked();
    void on_pbMergePdf_clicked();

    void on_pbFinish_clicked();

signals:
    void enableUndoSignal();
    void changeUndoSignal();
    void changeRedoSignal();
    void moveSlidersSignal();
};

#endif // MAINWINDOW_H

