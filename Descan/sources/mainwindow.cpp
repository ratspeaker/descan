#include "headers/mainwindow.h"
#include "headers/image.h"
#include "ui_mainwindow.h"
#include "headers/dialogmail.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Descan");

    display = new DisplayArea();
    display->getLabel()->resize(0, 0);
    display->getLabel()->setMouseTracking(true);
    display->getLabel()->installEventFilter(this);

    ui->scrollArea->setWidget(display->getLabel());
    ui->scrollArea->setAlignment(Qt::AlignCenter);

    pdf = new PDFHandler();

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
    QObject::connect(ui->pbNextEdit, &QPushButton::clicked, this, &MainWindow::cleanFirstPageLabel);
    QObject::connect(ui->pbNextFinish, &QPushButton::clicked, this, &MainWindow::showNextPage);

    ui->lblHelp->setText("This page provides basic photo editing.\n"
                             "\n"
                             "In section Resize & Rotate you can:\n"
                             " - scale your image horizontaly, verti-\n"
                             "   caly or in both directions\n"
                             " - rotate for 90 degrees, left or right\n"
                             " - crop image\n"
                             "In section Effects there are 4 sliders that you move and when you release them brightness, contrast, gamma correction and saturation will be changed. Also, there is grayscale option that transfers colored image to black and white one.\n"
                             "\n"
                             "Above these tools there are 5 buttons. You can click on them or use keyboard shortcuts:\n"
                             " - ctrl + Z = undo\n"
                             " - ctrl + shift + Z = redo\n"
                             " - ctrl + + = zoom in\n"
                             " - ctrl + - = zoom out\n"
                             " - F = fit (fits image into scroll area)\n"
                             "\n"
                             "For more information click");
    ui->lblLink->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    ui->lblLink->setOpenExternalLinks(true);
    ui->lblLink->setText("<a href=\"https://gitlab.com/matf-bg-ac-rs/course-rs/projects-2020-2021/07-descan/-/raw/master/Presentation/video.mp4\">here.</a>");
}

MainWindow::~MainWindow()
{
    delete pdf;
    if (drive)
        delete drive;
    delete display;
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
    //ciscenje dugmica i labela
    cleanDisplayArea();
    clearSliderValues();
    resetZoomButtons();
    resetUndoRedoButtons();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Image"), "/home/", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));

    if (!fileName.isEmpty()) {
        display->setElements(QStringList(fileName));

        QImage image = display->getElement()->getImage();
        if (image.isNull()) {
            QMessageBox::information(this, tr("Descan"), tr("Cannot load image."));
            return;
        }

        ui->lblOneImage->setText("image added");
        ui->lblMultiple->setText("");

        ui->pbNextEdit->setDisabled(false);
        display->setImageInLabel();
        display->getLabel()->adjustSize();
    }

    convertArg = "image has";
}

//ucitavanje vise slika
void MainWindow::on_pbImportMultiple_clicked()
{
    //ciscenje dugmica i labela
    cleanDisplayArea();
    clearSliderValues();
    resetZoomButtons();
    resetUndoRedoButtons();

    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Import Images"), "/home/", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
    fileNames.sort();

    if (1==fileNames.size())
    {
        QMessageBox::warning(this, tr("Descan"), tr("You cannot choose one image! Try with more."));
        return;
    }

    if (!fileNames.isEmpty()) {
         display->setElements(fileNames);

         vector<Image*> images = display->getElements();
         for (auto i=images.cbegin(); i != images.cend(); i++) {
             QImage image = (*i)->getImage();
             if (image.isNull()) {
                 QMessageBox::information(this, tr("Descan"), tr("Cannot load images."));
                 return;
             }
         }

         ui->lblMultiple->setText(tr("%1 images added").arg(fileNames.size()));
         ui->lblOneImage->setText("");

         ui->pbNextEdit->setDisabled(false);
         display->setImageInLabel();
         display->getLabel()->adjustSize();
         ui->pbRightImage->setDisabled(false);
    }

    convertArg = "images have";
}

//skaliranje slike u zavisnosti od pozicije slajdera
void MainWindow::on_hsScale_sliderMoved(int position)
{
    currentPosition = position;
    emit enableUndoSignal();
    double resizeFactor = 0.4;
    if (position) {
        resizeFactor = 0.4 + (2-0.4)*position/100;
    }
    //promena se manifestuje na kopiji i ta kopija se prikazuje u labeli
    imageCopy = display->getElement()->resizeImage(resizeFactor, 's');
    display->m_label->setPixmap(QPixmap::fromImage(imageCopy));
}

//trajno se cuva promena na objektu slike
void MainWindow::on_hsScale_sliderReleased()
{
    display->getElement()->saveAction();
    display->getElement()->setSlider("scale", currentPosition);
    display->setImageInLabel(imageCopy);
}

//menja se visina u zavisnosti od pozicije slajdera
void MainWindow::on_hsHorizontal_sliderMoved(int position)
{
    currentPosition = position;
    emit enableUndoSignal();
    double resizeFactor = 0.4;
    if (position) {
        resizeFactor = 0.4 + (2-0.4)*position/100;
    }
    //promena se manifestuje na kopiji i ta kopija se prikazuje u labeli
    imageCopy = display->getElement()->resizeImage(resizeFactor, 'w');
    display->m_label->setPixmap(QPixmap::fromImage(imageCopy));
}

//trajno se cuva promena na objektu slike
void MainWindow::on_hsHorizontal_sliderReleased()
{
    display->getElement()->saveAction();
    display->getElement()->setSlider("hor", currentPosition);
    display->setImageInLabel(imageCopy);
}

//menja se sirina u zavisnosti od pozicije slajdera
void MainWindow::on_hsVertical_sliderMoved(int position)
{
    currentPosition = position;
    emit enableUndoSignal();
    double resizeFactor = 0.4;
    if (position) {
        resizeFactor = 0.4 + (2-0.4)*position/100;
    }
    //promena se manifestuje na kopiji i ta kopija se prikazuje u labeli
    imageCopy = display->getElement()->resizeImage(resizeFactor, 'h');
    display->m_label->setPixmap(QPixmap::fromImage(imageCopy));
}

//trajno se cuva promena na objektu slike
void MainWindow::on_hsVertical_sliderReleased()
{
    display->getElement()->saveAction();
    display->getElement()->setSlider("ver", currentPosition);
    display->setImageInLabel(imageCopy);
}

//efekti
void MainWindow::on_pbGreyscale_clicked()
{
    emit enableUndoSignal();
    display->getElement()->saveAction();
    imageCopy = display->getElement()->greyScale();
    display->setImageInLabel(imageCopy);
}

void MainWindow::on_hsBrightness_sliderMoved(int position)
{
    currentPosition = position;
    emit enableUndoSignal();
    double brightnessFactor = (((position * (100 - (-100)))) / 100) -100;
    imageCopy = display->getElement()->changeBrightness(brightnessFactor);
    display->m_label->setPixmap(QPixmap::fromImage(imageCopy));
}

void MainWindow::on_hsBrightness_sliderReleased()
{
    display->getElement()->saveAction();
    display->getElement()->setSlider("brigh", currentPosition);
    display->setImageInLabel(imageCopy);
}

void MainWindow::on_hsContrast_sliderMoved(int position)
{
    currentPosition = position;
    emit enableUndoSignal();
    double contrastFactor = (((position * (100 - (-100)))) / 100) -100;
    imageCopy = display->getElement()->changeContrast(contrastFactor);
    display->m_label->setPixmap(QPixmap::fromImage(imageCopy));
}

void MainWindow::on_hsContrast_sliderReleased()
{
    display->getElement()->saveAction();
    display->getElement()->setSlider("con", currentPosition);
    display->setImageInLabel(imageCopy);
}

void MainWindow::on_hsCorrection_sliderMoved(int position)
{
    currentPosition = position;
    emit enableUndoSignal();
    imageCopy = display->getElement()->gammaCorrection(position*0.02);
    display->m_label->setPixmap(QPixmap::fromImage(imageCopy));
}

void MainWindow::on_hsCorrection_sliderReleased()
{
    display->getElement()->saveAction();
    display->getElement()->setSlider("gam", currentPosition);
    display->setImageInLabel(imageCopy);
}

void MainWindow::on_hsSaturation_sliderMoved(int position)
{
    currentPosition = position;
    emit enableUndoSignal();
    imageCopy = display->getElement()->changeSaturation(position*0.02);
    display->m_label->setPixmap(QPixmap::fromImage(imageCopy));
}

void MainWindow::on_hsSaturation_sliderReleased()
{
    display->getElement()->saveAction();
    display->getElement()->setSlider("sat", currentPosition);
    display->setImageInLabel(imageCopy);
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
    display->setImageInLabel();
    emit moveSlidersSignal();
}

//dugme koje obradjuje akciju redo
void MainWindow::on_tbRedo_clicked()
{
    display->getElement()->redoAction();
    emit changeRedoSignal();
    display->setImageInLabel();
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

void MainWindow::cleanFirstPageLabel()
{
    ui->lblOneImage->clear();
    ui->lblMultiple->clear();
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

void MainWindow::clearSliderValues()
{
    ui->hsBrightness->setValue(0);
    ui->hsContrast->setValue(0);
    ui->hsCorrection->setValue(0);
    ui->hsHorizontal->setValue(0);
    ui->hsSaturation->setValue(0);
    ui->hsScale->setValue(0);
    ui->hsVertical->setValue(0);
}

void MainWindow::resetZoomButtons()
{
    ui->tbZoomIn->setDisabled(false);
    ui->tbZoomOut->setDisabled(false);
}

void MainWindow::resetUndoRedoButtons()
{
    display->setToBeginning();
    for (unsigned i = 0; i < display->getSize(); i++) {
        Image* currentImage = display->getElement();
        currentImage->emptyUndoActions();
        currentImage->emptyRedoActions();
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
    checkZoomButtons();
}

void MainWindow::checkZoomButtons()
{
    ui->tbZoomIn->setDisabled(display->getElement()->getScaleFactor() > 2);
    ui->tbZoomOut->setDisabled(display->getElement()->getScaleFactor() < 0.4);
}

void MainWindow::on_tbZoomIn_clicked()
{
    display->scaleImage(1.25);
    checkZoomButtons();
}

void MainWindow::on_tbZoomOut_clicked()
{
    display->scaleImage(0.80);
    checkZoomButtons();
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

    checkZoomButtons();

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

    checkZoomButtons();

    emit changeUndoSignal();
    emit changeRedoSignal();
    emit moveSlidersSignal();
}

//konvertovanje jedne ili vise slika u pdf
void MainWindow::on_pbConvert_clicked()
{
    QString username = QString::fromStdString(getlogin());
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF File As"),
                                                          "/home/"+username,
                                                          tr("PDF Files(*.pdf)"));

    if (!fileName.isEmpty()) {
        QString newFileName = fileName + ".pdf";
        PDFHandler::convertImagesIntoPdf(newFileName, display->m_elements);
        filePathsPdf.append(newFileName);
        QMessageBox::information(this, tr("Convert to PDF"),
                                       tr("Your %1 been successfully converted!").arg(convertArg));

        //nakon konvertovanja, dodatne opcije su omogucene
        enableOptions();
    }
}

void MainWindow::enableOptions() {
    ui->pbCompress->setDisabled(false);
    ui->pbDrive->setDisabled(false);
    ui->pbMail->setDisabled(false);
}

//jedan pdf dokument delimo u vise
void MainWindow::on_pbSplitPdf_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import PDF"), "/home/", tr("*.pdf"));

    if (!fileName.isEmpty()) {
        pdf->setInputFileSplit(fileName);
        filePathsPdf = pdf->splitPdf();

        if (!filePathsPdf.isEmpty()) {
            ui->stackedWidget->setCurrentIndex(2);
            ui->pbBackEdit->deleteLater();
            ui->pbConvert->deleteLater();
            enableOptions();
        }
    }
}

//vise dokumenata objedinjujemo u jedan
void MainWindow::on_pbMergePdf_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Import PDFs"), "/home/", tr("*.pdf"));
    fileNames.sort();

    if (!fileNames.isEmpty()) {
        pdf->setInputFilesMerge(fileNames);
        QString filePath = pdf->mergePdf();
        filePathsPdf.append(filePath);

        if (!filePath.isEmpty()) {
            ui->stackedWidget->setCurrentIndex(2);
            ui->pbBackEdit->deleteLater();
            ui->pbConvert->deleteLater();
            enableOptions();
        }
    }
}

void MainWindow::on_pbFinish_clicked()
{
    close();
}

//dugme za kompresovanje dokumenta
void MainWindow::on_pbCompress_clicked()
{
    if (!filePathsPdf.isEmpty()) {
        for (auto &path: filePathsPdf) {
            PDFHandler::compressPDF(path);
        }
        QMessageBox::information(this, tr("Compress PDF"),
                                       tr("Your files have been successfully compressed!"));
    }
}

//dugme za slanje mejla
void MainWindow::on_pbMail_clicked()
{
    DialogMail dialogMail(this, std::move(filePathsPdf));
    dialogMail.setModal(true);
    dialogMail.exec();
}

//dugme za postavljanje na drajv
void MainWindow::on_pbDrive_clicked()
{
    drive = new Drive();
    drive->uploadToDrive(filePathsPdf);
    connect(drive, &Drive::endConnectSignal, this, [=](){
        QMessageBox::information(this, tr("Upload files"),
                                       tr("Your files have been successfully uploaded to Google Drive!"));
    });
}
