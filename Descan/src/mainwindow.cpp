#include "headers/mainwindow.h"
#include "headers/image.h"
#include "ui_mainwindow.h"
#include<QPdfWriter>
#include<QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    display = new DisplayArea();

    display->getLabel()->resize(0, 0);
    display->getLabel()->setMouseTracking(true);
    display->getLabel()->installEventFilter(this);

    ui->scrollArea->setWidget(display->getLabel());
    ui->scrollArea->setAlignment(Qt::AlignCenter);

    ui->pbNextEdit->setDisabled(true);

    //ako se bilo šta promeni na slici, operacija undo ce biti omogućena
    QObject::connect(this, &MainWindow::enableUndoSignal, this, &MainWindow::enableUndo);

    //za svaku sliku posebno se koristi zaseban undo/redo
    QObject::connect(this, &MainWindow::changeUndoSignal, this, &MainWindow::changeUndoState);
    QObject::connect(this, &MainWindow::changeRedoSignal, this, &MainWindow::changeRedoState);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbNextEdit_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pbNextFinish_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pbBackStart_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    /* Treba osloboditi prostor za nove slike */
    display->freeImages();

}

void MainWindow::on_pbBackEdit_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pbImport_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Image"), "/home/");

    if (!fileName.isEmpty()) {
         display->setElements(QStringList(fileName));

         QImage image = display->getElement()->getImage();
         qDebug() << "dimenzije slike su " << image.size().width() << image.size().height();

         if (image.isNull()) {
             QMessageBox::information(this, tr("Descan"), tr("Cannot load image."));
             return;
         }

         ui->pbNextEdit->setDisabled(false);
         setCursor(Qt::ArrowCursor);
         display->setImageInLabel();
         display->getLabel()->adjustSize();
    }
}

void MainWindow::on_pbImportMultiple_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Import Images"), "/home/");

    if (!fileNames.isEmpty()) {
         display->setElements(fileNames);

         QImage image = display->getElement()->getImage();
         qDebug() << "dimenzije slike su " << image.size().width() << image.size().height();

         if (image.isNull()) {
             QMessageBox::information(this, tr("Descan"), tr("Cannot load images."));
             return;
         }

         ui->pbNextEdit->setDisabled(false);
         setCursor(Qt::ArrowCursor);
         display->setImageInLabel();
         display->getLabel()->adjustSize();
    }
}

//TODO: treba izmeniti da ne menja direktno sliku (kopija objekta ili naci bolje resenje?)
/*Funkcija koja skalira sliku u zavisnosti od pozicije slajdera*/
void MainWindow::on_hsScale_sliderMoved(int position)
{
    emit enableUndoSignal();
    double resizeFactor = 0.4;
    if (position) {
        resizeFactor = 0.4 + (2-0.4)*position/100;
    }
    /* Promena se manifestuje na kopiji i ta kopija se prikazuje u labeli */
    image_copy = display->getElement()->resizeImage(resizeFactor, 's');
    display->m_label->setPixmap(QPixmap::fromImage(image_copy));
}
/*Funkcija koja cuva trajno promenu na objektu slike */
/*TODO: posto se ove dve akcije cesto koriste -> izdvojiti ih u funkciju */
void MainWindow::on_hsScale_sliderReleased()
{
    display->getElement()->saveAction();
    display->setImageInLabel(image_copy);
}
/*Menja visinu u zavisnosti od parametra*/
void MainWindow::on_hsHorizontal_sliderMoved(int position)
{
    emit enableUndoSignal();
    double resizeFactor = 0.4;
    /*Odredjuje faktor na osnovu pozicije slajdera*/
    if (position) {
        resizeFactor = 0.4 + (2-0.4)*position/100;
    }
    /*Cuva u kopiji i tu kopiju prikazuje*/
    image_copy = display->getElement()->resizeImage(resizeFactor, 'w');
    display->m_label->setPixmap(QPixmap::fromImage(image_copy));
}
/*Funkcija koja kada se otpusti slajder cuva promenu u objekat slike*/
void MainWindow::on_hsHorizontal_sliderReleased()
{
    display->getElement()->saveAction();
    display->setImageInLabel(image_copy);
}
/*Menja sirinu u zavisnosti od parametra*/
void MainWindow::on_hsVertical_sliderMoved(int position)
{
    emit enableUndoSignal();
    double resizeFactor = 0.4;
    if (position) {
        resizeFactor = 0.4 + (2-0.4)*position/100;
    }

    image_copy = display->getElement()->resizeImage(resizeFactor, 'h');
    display->m_label->setPixmap(QPixmap::fromImage(image_copy));
}
/*Funkcija koja kada se otpusti slajder cuva promene u objekat slike */
void MainWindow::on_hsVertical_sliderReleased()
{
    display->getElement()->saveAction();
    display->setImageInLabel(image_copy);
}

void MainWindow::on_pbGreyscale_clicked()
{
    emit enableUndoSignal();
    display->getElement()->saveAction();
    image_copy = display->getElement()->greyScale();
    display->setImageInLabel(image_copy);
}

void MainWindow::on_hsBrightness_sliderMoved(int position)
{
    emit enableUndoSignal();
    double brightnessFactor = (((position * (100 - (-100)))) / 100) -100;
    image_copy = display->getElement()->changeBrightness(brightnessFactor);
    display->m_label->setPixmap(QPixmap::fromImage(image_copy)); //prikazuje se samo privremena kopija
}

void MainWindow::on_hsBrightness_sliderReleased()
{
    display->getElement()->saveAction();
    display->setImageInLabel(image_copy);
}

void MainWindow::on_hsContrast_sliderMoved(int position)
{
    emit enableUndoSignal();
    double contrastFactor = (((position * (100 - (-100)))) / 100) -100;
    image_copy = display->getElement()->changeContrast(contrastFactor);
    display->m_label->setPixmap(QPixmap::fromImage(image_copy)); //prikazuje se samo privremena kopija
}

void MainWindow::on_hsContrast_sliderReleased()
{
    display->getElement()->saveAction();
    display->setImageInLabel(image_copy);
}

void MainWindow::on_hsCorrection_sliderMoved(int position)
{
    emit enableUndoSignal();
    image_copy = display->getElement()->gammaCorrection(position*0.02);
    display->m_label->setPixmap(QPixmap::fromImage(image_copy)); //prikazuje se samo privremena kopija
}

void MainWindow::on_hsCorrection_sliderReleased()
{
    display->getElement()->saveAction();
    display->setImageInLabel(image_copy);
}

void MainWindow::on_hsSaturation_sliderMoved(int position)
{
    emit enableUndoSignal();
    image_copy = display->getElement()->changeSaturation(position*0.02);
    display->m_label->setPixmap(QPixmap::fromImage(image_copy)); //prikazuje se samo privremena kopija
    //display->scaleImage(1.0);
}

void MainWindow::on_hsSaturation_sliderReleased()
{
    display->getElement()->saveAction();
    display->setImageInLabel(image_copy);
}
/*Funkcija za omogucavanje klika na dugme Undo*/
void MainWindow::enableUndo()
{
    ui->tbUndo->setDisabled(false);
}

/* Funkcija koja obradjuje akciju Undo */
void MainWindow::on_tbUndo_clicked()
{   /* Vracamo prethodno stanje slike */
    display->getElement()->undoAction();
    qDebug() << "undoStack: " << display->getElement()->undoStack.size() << " " << display->getElement()->redoStack.size();
    qDebug() << "redoStack: " << display->getElement()->redoStack.size() << " " << display->getElement()->undoStack.size();

    emit changeUndoSignal();
     /* Prikazuje se u labeli prethodna izmena slike */
    display->setImageInLabel();
}

/* Funkcija koja obradjuje akciju Redo*/
void MainWindow::on_tbRedo_clicked()
{
    display->getElement()->redoAction();
    qDebug() << "redoStack: " << display->getElement()->redoStack.size() << " " << display->getElement()->undoStack.size();
    qDebug() << "undoStack: " << display->getElement()->undoStack.size() << " " << display->getElement()->redoStack.size();

    emit changeRedoSignal();

    display->setImageInLabel();
}

/* Funkcija za cropovanje slike */
void MainWindow::on_tbCrop_clicked()
{
    display->getElement()->saveAction();
    emit enableUndoSignal();
    cropPressed = true;
}

//prati događaje miša koji se dešavaju kada se pređe preko labele sa slikom
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == display->getLabel() && cropPressed!=false) {
        if (event->type()== QEvent::MouseButtonPress) {
            QMouseEvent *newEvent = static_cast<QMouseEvent*>(event);

            //pamtimo gornje levo teme pravougaonika
            startPoint = newEvent->pos();
            qDebug() << startPoint;

            rubberBandCreated = true;
            setCursor(Qt::CrossCursor);

            rubberBand = new QRubberBand(QRubberBand::Rectangle, display->getLabel());
            rubberBand->setGeometry(QRect(startPoint, QSize()));
            rubberBand->show();
        }
        else if (event->type() == QEvent::MouseMove) {
            QMouseEvent *newEvent = static_cast<QMouseEvent*>(event);
            setCursor(Qt::CrossCursor);
            if (rubberBandCreated) {
                rubberBand->setGeometry(QRect(startPoint, newEvent->pos()));
                rubberBand->show();
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease) {
            //display->getElement()->saveAction();
            QMouseEvent *newEvent = static_cast<QMouseEvent*>(event);
            setCursor(Qt::ArrowCursor);

            //pamtimo gornje levo teme pravougaonika
            endPoint = newEvent->pos();

            if (rubberBandCreated)
                rubberBand->deleteLater();

            //sečemo selektovani deo
            display->getElement()->cropImage(startPoint/display->getElement()->getScaleFactor(),
                                             endPoint/display->getElement()->getScaleFactor());
            display->setImageInLabel();

            rubberBandCreated = false;
            cropPressed = false;
        }
        return true;
    }
    return false;
}

/*Funkcija koja rotira sliku ulevo */
void MainWindow::on_tbRotateLeft_clicked()
{
    emit enableUndoSignal();
    display->getElement()->saveAction();
    display->getElement()->rotateImage(-90);
    display->setImageInLabel();
}

/*Funkcija koja rotira sliku udesno */
void MainWindow::on_tbRotateRight_clicked()
{
    emit enableUndoSignal();
    display->getElement()->saveAction();
    display->getElement()->rotateImage(90);
    display->setImageInLabel();
}

/* Funkcija koja fituje sliku u Skrol deo */
void MainWindow::on_tbFit_clicked()
{   /*Uzimamo velicine skrol dela i piksmape slike */
    QSize scrollSize = ui->scrollArea->size();
    QSize pixmapSize = QPixmap::fromImage(display->getElement()->getImage()).size();
    /*Skaliramo sliku u skrol deo tako da ne bude distorzije */
    double imageRatio = static_cast<double>(pixmapSize.height()) / pixmapSize.width();

    double scaleBy;
    if (scrollSize.width() * imageRatio > scrollSize.height()) {
        scaleBy = static_cast<double>(scrollSize.height()) / pixmapSize.height();
    }
    else {
        scaleBy = static_cast<double>(scrollSize.width()) / pixmapSize.width();
    }

    display->scaleImage(scaleBy/display->getElement()->getScaleFactor());
    /*Dugmici za uvecanje i smanjenje ce biti omoguceni ako je ispunjen uslov */
    ui->tbZoomIn->setEnabled(display->getElement()->getScaleFactor() < 2);
    ui->tbZoomOut->setEnabled(display->getElement()->getScaleFactor() > 0.4);
    qDebug() << display->getElement()->getScaleFactor();
}

/* Funkcija koja uvecava sliku */
void MainWindow::on_tbZoomIn_clicked()
{   /*Skalira se za odredjen faktor koji ce uvecavati*/
    display->scaleImage(1.25);
    /* Dugmici za ZoomIn i ZoomOut se onemogucavaju ako je neki uslov ispunjen */
    ui->tbZoomIn->setDisabled(display->getElement()->getScaleFactor() > 2);
    ui->tbZoomOut->setEnabled(display->getElement()->getScaleFactor() > 0.4);
}

/*Funkcija koja smanjuje sliku */
void MainWindow::on_tbZoomOut_clicked()
{   /*Skalira se za odredjeni faktor koji ce je smanjiti */
    display->scaleImage(0.80);
    /* Dugmici za ZoomIn i ZoomOut se onemogucavaju ako je neki uslov ispunjen */
    ui->tbZoomOut->setDisabled(display->getElement()->getScaleFactor() < 0.4);
    ui->tbZoomIn->setEnabled(display->getElement()->getScaleFactor() < 2);
}

/* Prelazi se na prethodnu sliku */
void MainWindow::on_pbLeftImage_clicked()
{   /*Prelazi se na prethodni element u vektoru*/
    display->getPreviousElement();
    /*Prikazuje se slika u labelu */
    display->setImageInLabel();
    /*Omogucavaju se dugmici za uvecavanje i umanjenje slike, uz zadovoljen uslov */
    ui->tbZoomIn->setEnabled(display->getElement()->getScaleFactor() < 2);
    ui->tbZoomOut->setEnabled(display->getElement()->getScaleFactor() > 0.4);
   /* Debug ispis
    * qDebug() << "prev" << display->getElement()->getScaleFactor(); */

    /*Signali za undo i redo */
    emit changeUndoSignal();
    emit changeRedoSignal();
}

/* Prelazi na narednu sliku */
void MainWindow::on_pbRightImage_clicked()
{
   /*Pomera iterator na naredni element */
    display->getNextElement();
    /*Prikazuje se slika u labeli */
    display->setImageInLabel();
    /*Omogucavaju se dugmici za uvecavanje i umanjenje slike, uz zadovoljen uslov */
    ui->tbZoomIn->setEnabled(display->getElement()->getScaleFactor() < 2);
    ui->tbZoomOut->setEnabled(display->getElement()->getScaleFactor() > 0.4);
    /* Debug ispis
     * qDebug() << "next" << display->getElement()->getScaleFactor();
     */

    /*Signali za undo i redo */
    emit changeUndoSignal();
    emit changeRedoSignal();
}
/*Funkcija koja ako nema vise akcija na koje se vraca stavlja da je onemoguceno pritiskanje undo dugmeta *
 *kao i obrnuto za dugme Redo */
void MainWindow::changeUndoState()
{
    if ((display->getElement()->undoStack.size())==0) {
        ui->tbUndo->setDisabled(true);
    }
    else {
        if ((display->getElement()->redoStack.size())!=0) {
           ui->tbRedo->setDisabled(false);
        }
        ui->tbUndo->setDisabled(false);
    }
}
/*Funkcija koja omogucava pritiskanje redo dugmeta ako ne postoji akcija unapred na koju mozemo da se vratimo */
void MainWindow::changeRedoState()
{
    if (display->getElement()->redoStack.size()==0) {
        ui->tbRedo->setDisabled(true);
    }
    else {
        if ((display->getElement()->undoStack.size())!=0) {
            ui->tbUndo->setDisabled(false);
        }
        ui->tbRedo->setDisabled(false);
    }
}
