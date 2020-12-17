#include "headers/mainwindow.h"
#include "headers/image.h"
#include "ui_mainwindow.h"

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

    //ako se bilo sta promeni na slici, operacija undo ce biti omogucena
    QObject::connect(this, &MainWindow::enableUndoSignal, this, &MainWindow::enableUndo);

    //za svaku sliku posebno se koristi zaseban undo/redo
    QObject::connect(this, &MainWindow::changeUndoSignal, this, &MainWindow::changeUndoState);
    QObject::connect(this, &MainWindow::changeRedoSignal, this, &MainWindow::changeRedoState);

    //pomeraju se slajderi u zavisnosti da li se promeni slika ili se pritisnu undo/redo
    QObject::connect(this, &MainWindow::moveSlidersSignal, this, &MainWindow::moveSliders);

    QObject::connect(ui->pbBackEdit, &QPushButton::clicked, this, &MainWindow::showPreviousPage);
    QObject::connect(ui->pbBackStart, &QPushButton::clicked, this, &MainWindow::showPreviousPage);
    QObject::connect(ui->pbNextEdit, &QPushButton::clicked, this, &MainWindow::showNextPage);
    QObject::connect(ui->pbNextFinish, &QPushButton::clicked, this, &MainWindow::showNextPage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//implementacija slotova za prelazak na prethodnu/narednu stranu
void MainWindow::showPreviousPage()
{
    int currentIndex = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(--currentIndex);
}

void MainWindow::showNextPage()
{
    int currentIndex = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(++currentIndex);
}

//ucitavanje jedne slike
void MainWindow::on_pbImport_clicked()
{
    cleanDisplayArea();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Image"), "/home/");

    if (!fileName.isEmpty()) {
         display->setElements(QStringList(fileName));

         QImage image = display->getElement()->getImage();
         //qDebug() << "dimenzije slike su " << image.size().width() << image.size().height();

         if (image.isNull()) {
             QMessageBox::information(this, tr("Descan"), tr("Cannot load image."));
             return;
         }

         ui->pbNextEdit->setDisabled(false);
         setCursor(Qt::ArrowCursor);
         display->setImageInLabel();
         display->getLabel()->adjustSize();
         ui->pbLeftImage->setDisabled(true);
         ui->pbRightImage->setDisabled(true);
    }
}

//ucitavanje vise slika
void MainWindow::on_pbImportMultiple_clicked()
{
    cleanDisplayArea();
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Import Images"), "/home/");
    fileNames.sort();

    if (!fileNames.isEmpty()) {
         display->setElements(fileNames);

         QImage image = display->getElement()->getImage();

         if (image.isNull()) {
             QMessageBox::information(this, tr("Descan"), tr("Cannot load images."));
             return;
         }

         ui->pbNextEdit->setDisabled(false);
         setCursor(Qt::ArrowCursor);
         display->setImageInLabel();
         display->getLabel()->adjustSize();
         ui->pbLeftImage->setDisabled(true);
         ui->pbRightImage->setDisabled(false);
    }
}

//TODO: treba izmeniti da ne menja direktno sliku (kopija objekta ili naci bolje resenje?)
//funkcija koja skalira sliku u zavisnosti od pozicije slajdera
void MainWindow::on_hsScale_sliderMoved(int position)
{
    poss = position;
    emit enableUndoSignal();
    double resizeFactor = 0.4;
    if (position) {
        resizeFactor = 0.4 + (2-0.4)*position/100;
    }
    //promena se manifestuje na kopiji i ta kopija se prikazuje u labeli
    image_copy = display->getElement()->resizeImage(resizeFactor, 's');
    display->m_label->setPixmap(QPixmap::fromImage(image_copy));
}

//funkcija koja cuva trajno promenu na objektu slike
//TODO: posto se ove dve akcije cesto koriste -> izdvojiti ih u funkciju
void MainWindow::on_hsScale_sliderReleased()
{
    display->getElement()->saveAction();
    display->getElement()->setSlider("scale", poss);
    display->setImageInLabel(image_copy);
}

//menja visinu u zavisnosti od parametra
void MainWindow::on_hsHorizontal_sliderMoved(int position)
{
    poss = position;
    emit enableUndoSignal();
    double resizeFactor = 0.4;

    //odredjuje faktor na osnovu pozicije slajdera
    if (position) {
        resizeFactor = 0.4 + (2-0.4)*position/100;
    }

    //cuva u kopiji i tu kopiju prikazuje
    image_copy = display->getElement()->resizeImage(resizeFactor, 'w');
    display->m_label->setPixmap(QPixmap::fromImage(image_copy));
}

//funkcija koja kada se otpusti slajder cuva promenu u objekat slike
void MainWindow::on_hsHorizontal_sliderReleased()
{
    display->getElement()->saveAction();
    display->getElement()->setSlider("hor", poss);
    display->setImageInLabel(image_copy);
}

//menja sirinu u zavisnosti od parametra
void MainWindow::on_hsVertical_sliderMoved(int position)
{
    poss = position;
    emit enableUndoSignal();
    double resizeFactor = 0.4;

    //odredjuje faktor na osnovu pozicije slajdera
    if (position) {
        resizeFactor = 0.4 + (2-0.4)*position/100;
    }

    //cuva u kopiji i tu kopiju prikazuje
    image_copy = display->getElement()->resizeImage(resizeFactor, 'h');
    display->m_label->setPixmap(QPixmap::fromImage(image_copy));
}

//funkcija koja kada se otpusti slajder cuva promene u objekat slike
void MainWindow::on_hsVertical_sliderReleased()
{
    display->getElement()->saveAction();
    display->getElement()->setSlider("ver", poss);
    display->setImageInLabel(image_copy);
}

//efekti
void MainWindow::on_pbGreyscale_clicked()
{
    emit enableUndoSignal();
    display->getElement()->saveAction();
    image_copy = display->getElement()->greyScale();
    display->setImageInLabel(image_copy);
}

void MainWindow::on_hsBrightness_sliderMoved(int position)
{
    poss = position;
    emit enableUndoSignal();
    double brightnessFactor = (((position * (100 - (-100)))) / 100) -100;
    image_copy = display->getElement()->changeBrightness(brightnessFactor);
    display->m_label->setPixmap(QPixmap::fromImage(image_copy)); //prikazuje se samo privremena kopija
}

void MainWindow::on_hsBrightness_sliderReleased()
{
    display->getElement()->saveAction();
    display->getElement()->setSlider("brigh", poss);
    display->setImageInLabel(image_copy);
}

void MainWindow::on_hsContrast_sliderMoved(int position)
{
    poss = position;
    emit enableUndoSignal();
    double contrastFactor = (((position * (100 - (-100)))) / 100) -100;
    image_copy = display->getElement()->changeContrast(contrastFactor);
    display->m_label->setPixmap(QPixmap::fromImage(image_copy)); //prikazuje se samo privremena kopija
}

void MainWindow::on_hsContrast_sliderReleased()
{
    display->getElement()->saveAction();
    display->getElement()->setSlider("con", poss);
    display->setImageInLabel(image_copy);
}

void MainWindow::on_hsCorrection_sliderMoved(int position)
{
    poss = position;
    emit enableUndoSignal();
    image_copy = display->getElement()->gammaCorrection(position*0.02);
    display->m_label->setPixmap(QPixmap::fromImage(image_copy)); //prikazuje se samo privremena kopija
}

void MainWindow::on_hsCorrection_sliderReleased()
{
    display->getElement()->saveAction();
    display->getElement()->setSlider("gam", poss);
    display->setImageInLabel(image_copy);
}

void MainWindow::on_hsSaturation_sliderMoved(int position)
{
    poss = position;
    emit enableUndoSignal();
    image_copy = display->getElement()->changeSaturation(position*0.02);
    display->m_label->setPixmap(QPixmap::fromImage(image_copy)); //prikazuje se samo privremena kopija
    //display->scaleImage(1.0);
}

void MainWindow::on_hsSaturation_sliderReleased()
{
    display->getElement()->saveAction();
    display->getElement()->setSlider("sat", poss);
    display->setImageInLabel(image_copy);
}

//funkcija za omogucavanje klika na dugme undo
void MainWindow::enableUndo()
{
    ui->tbUndo->setDisabled(false);
}

//dugme koje obradjuje akciju undo
void MainWindow::on_tbUndo_clicked()
{
    display->getElement()->undoAction();
    emit changeUndoSignal();

    //prikazujemo sliku u labeli
    display->setImageInLabel();

    emit moveSlidersSignal();
}

//dugme koje obradjuje akciju redo
void MainWindow::on_tbRedo_clicked()
{
    display->getElement()->redoAction();
    emit changeRedoSignal();

    //prikazujemo sliku u labeli
    display->setImageInLabel();

    emit moveSlidersSignal();
}

//dugme koje omogucava crop slike
void MainWindow::on_tbCrop_clicked()
{
    display->getElement()->saveAction();
    emit enableUndoSignal();
    cropPressed = true;
}

//prati dogadjaje misa koji se desavaju kada se predje preko labele sa slikom
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == display->getLabel() && cropPressed != false) {
        if (event->type() == QEvent::MouseButtonPress) {
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
            QMouseEvent *newEvent = static_cast<QMouseEvent*>(event);
            setCursor(Qt::ArrowCursor);

            //pamtimo donje desno teme pravougaonika
            endPoint = newEvent->pos();

            if (rubberBandCreated)
                rubberBand->deleteLater();

            //secemo selektovani deo
            display->getElement()->cropImage(startPoint / display->getElement()->getScaleFactor(),
                                             endPoint / display->getElement()->getScaleFactor());

            //prikazujemo sliku u labeli
            display->setImageInLabel();

            rubberBandCreated = false;
            cropPressed = false;
        }
        return true;
    }
    return false;
}

void MainWindow::cleanDisplayArea()
{
    display->freeImages();
}

void MainWindow::moveSliders()
{
    for (auto it: display->getElement()->getSlider()) {
        if (it.first=="scale")
            ui->hsScale->setValue(it.second);
        else if (it.first=="hor")
            ui->hsHorizontal->setValue(it.second);
        else if (it.first=="ver")
            ui->hsVertical->setValue(it.second);
        else if (it.first=="brigh")
            ui->hsBrightness->setValue(it.second);
        else if (it.first=="con")
            ui->hsContrast->setValue(it.second);
        else if (it.first=="gam")
            ui->hsCorrection->setValue(it.second);
        else if (it.first=="sat")
            ui->hsSaturation->setValue(it.second);
    }
}

//rotiramo sliku ulevo
void MainWindow::on_tbRotateLeft_clicked()
{
    emit enableUndoSignal();
    display->getElement()->saveAction();
    display->getElement()->rotateImage(-90);
    display->setImageInLabel();
}

//rotiramo sliku udesno
void MainWindow::on_tbRotateRight_clicked()
{
    emit enableUndoSignal();
    display->getElement()->saveAction();
    display->getElement()->rotateImage(90);
    display->setImageInLabel();
}

//fitujemo sliku u skrol deo
void MainWindow::on_tbFit_clicked()
{
    //uzimamo dimenzije skrol dela i piksmape
    QSize scrollSize = ui->scrollArea->size();
    QSize pixmapSize = QPixmap::fromImage(display->getElement()->getImage()).size();

    //vrsimo skaliranje piksmape
    double imageRatio = static_cast<double>(pixmapSize.height()) / pixmapSize.width();

    double scaleBy;
    if (scrollSize.width() * imageRatio > scrollSize.height()) {
        scaleBy = static_cast<double>(scrollSize.height()) / pixmapSize.height();
    }
    else {
        scaleBy = static_cast<double>(scrollSize.width()) / pixmapSize.width();
    }

    display->scaleImage(scaleBy/display->getElement()->getScaleFactor());

    //dugmici za zoom in i zoom out ce biti omoguceni ako je ispunjen neki uslov
    ui->tbZoomIn->setEnabled(display->getElement()->getScaleFactor() < 2);
    ui->tbZoomOut->setEnabled(display->getElement()->getScaleFactor() > 0.4);
    //qDebug() << display->getElement()->getScaleFactor();
}

void MainWindow::on_tbZoomIn_clicked()
{
    display->scaleImage(1.25);
    ui->tbZoomIn->setDisabled(display->getElement()->getScaleFactor() > 2);
    ui->tbZoomOut->setEnabled(display->getElement()->getScaleFactor() > 0.4);
}

void MainWindow::on_tbZoomOut_clicked()
{
    display->scaleImage(0.80);
    ui->tbZoomOut->setDisabled(display->getElement()->getScaleFactor() < 0.4);
    ui->tbZoomIn->setEnabled(display->getElement()->getScaleFactor() < 2);
}

//prelazimo na prethodnu sliku
void MainWindow::on_pbLeftImage_clicked()
{
    //prelazimo na prethodni element u vektoru
    display->getPreviousElement();

    //prikazujemo sliku u labeli
    display->setImageInLabel();

    //ako je prva slika
    if (display->getIndDisable() == 1)
        ui->pbLeftImage->setDisabled(true);
    ui->pbRightImage->setDisabled(false);

    //dugmici za zoom in i zoom out ce biti omoguceni ako je ispunjen neki uslov
    ui->tbZoomIn->setEnabled(display->getElement()->getScaleFactor() < 2);
    ui->tbZoomOut->setEnabled(display->getElement()->getScaleFactor() > 0.4);

    //signali za undo i redo
    emit changeUndoSignal();
    emit changeRedoSignal();
    emit moveSlidersSignal();
}

//prelazimo na narednu sliku
void MainWindow::on_pbRightImage_clicked()
{
    //prelazimo na naredni element u vektoru
    display->getNextElement();

    //prikazujemo sliku u labeli
    display->setImageInLabel();

    //ako je poslednja slika
    if (display->getIndDisable() == 2)
        ui->pbRightImage->setDisabled(true);
    ui->pbLeftImage->setDisabled(false);

    //dugmici za zoom in i zoom out ce biti omoguceni ako je ispunjen neki uslov
    ui->tbZoomIn->setEnabled(display->getElement()->getScaleFactor() < 2);
    ui->tbZoomOut->setEnabled(display->getElement()->getScaleFactor() > 0.4);

    //signali za undo i redo
    emit changeUndoSignal();
    emit changeRedoSignal();
    emit moveSlidersSignal();
}

//ako nema vise akcija na koje mozemo da se vratimo,
//stavljamo da je onemoguceno undo dugme, takodje vrsi se provera i za redo dugme
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

//ako nema vise akcija na koje mozemo da se vratimo unapred,
//stavljamo da je onemoguceno redo dugme, takodje vrsi se provera i za undo dugme
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

//konvertovanje jedne ili vise slika u PDF
void MainWindow::on_pbConvert_clicked()
{
    //trenutno ovde korisnik bira gde se cuva fajl
    //u kasnijim komitovima napraviti da se cuva kada korisnik pritisne dugme save
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF File As"),
                                                          "/home/",
                                                          tr("PDF Files(*.pdf)"));

    PDFHandler::convertImagesIntoPdf(fileName, display->getElements());
}

//jedan pdf dokument delimo u vise
void MainWindow::on_pbSplitPdf_clicked()
{
    PDFHandler* pdf = new PDFHandler();

    QString filePath = QFileDialog::getOpenFileName(this, tr("Import PDF"), "/home/",  tr("*.pdf"));

    pdf->setInputFileSplit(filePath);
    pdf->splitPdf();

    ui->stackedWidget->setCurrentIndex(2);

    delete pdf;
}

//vise dokumenata objedinjujemo u jedan
void MainWindow::on_pbMergePdf_clicked()
{
    PDFHandler* pdf = new PDFHandler();

    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Import PDFs"), "/home/", tr("*.pdf"));

    pdf->setInputFilesMerge(fileNames);
    pdf->mergePdf();

    ui->stackedWidget->setCurrentIndex(2);

    delete pdf;
}

//izlazimo iz programa
void MainWindow::on_pbFinish_clicked()
{
    close();
}
