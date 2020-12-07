#include "headers/mainwindow.h"
#include "headers/image.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*Ako se bilo šta promeni na slici, operacija undo ce biti omogućena.*/
    QObject::connect(this, &MainWindow::enableUndoSignal, this, &MainWindow::enableUndo);

    display = new DisplayArea();

    display->getLabel()->resize(0, 0);
    display->getLabel()->setMouseTracking(true);
    display->getLabel()->installEventFilter(this); //za crop

    ui->scrollArea->setWidget(display->getLabel());
     ui->pbNextEdit->setDisabled(true);
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
}

void MainWindow::on_pbBackEdit_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pbImport_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Image"), "/home/");
    if(fileName.isEmpty()){
        QMessageBox::critical(this, "Choose image", "Can't proceed if image not imported!");
    }
    else{
        ui->pbNextEdit->setDisabled(false);
    }

    if (!fileName.isEmpty()) {
         //img = Image();
         display->setElement(fileName);

         QImage image = display->getElement()->getImage();
         qDebug() << "dimenzije slike su " << image.size().width() << image.size().height();

         if (image.isNull()) {
             QMessageBox::information(this, tr("Descan"), tr("Cannot load image."));
             return;
         }
          ui->pbNextEdit->setDisabled(false);
         display->setScaleFactor(1.0);
         setCursor(Qt::ArrowCursor);

         display->setImageInLabel();
         display->getLabel()->adjustSize();
    }
}

/*
void MainWindow::on_pbImportMultiple_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            "Select one or more files to open",
                            "/home",
                            "Images (*.png *.xpm *.jpg)");
}

void MainWindow::on_hsBrightness_3_sliderMoved(int position)
{
    //qDebug() << position;
    cv::Mat image = m_img->LoadImage();
    cv::multiply(image, cv::Scalar(position/10, position/10, position/10), image);
    m_img->ScaleImage(image);
    QPixmap pixmap = m_img->DisplayImage(image);
    ui->labelImage->setPixmap(pixmap);
}
*/

void MainWindow::on_pbImportMultiple_clicked()
{

}

/*
void MainWindow::on_toolButton_5_clicked()
{

}

void MainWindow::on_toolButton_6_clicked()
{

}
*/
// TODO: treba izmeniti da ne menja direktno sliku(kopija objekta ili naci bolje resenje?)
void MainWindow::on_hsScale_2_sliderMoved(int position)
{
    emit enableUndoSignal();
    double resizeFactor = 0.4;
    if(position)
        resizeFactor = 0.4 + (2-0.4)*position/100;

    display->getElement()->resizeImage(resizeFactor, 's');
    display->setImageInLabel();
}

void MainWindow::on_hsHorizontal_2_sliderMoved(int position)
{
    emit enableUndoSignal();
    double resizeFactor = 0.4;
    if(position)
        resizeFactor = 0.4 + (2-0.4)*position/100;

    display->getElement()->resizeImage(resizeFactor, 'w');
    display->setImageInLabel();
}

void MainWindow::on_hsVertical_2_sliderMoved(int position)
{
    emit enableUndoSignal();
    double resizeFactor = 0.4;
    if(position)
        resizeFactor = 0.4 + (2-0.4)*position/100;

    display->getElement()->resizeImage(resizeFactor, 'h');
    display->setImageInLabel();
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
}

void MainWindow::on_hsSaturation_sliderReleased()
{
    display->getElement()->saveAction();
    display->setImageInLabel(image_copy);
}

void MainWindow::enableUndo()
{
    ui->toolButton->setDisabled(false);
}

/*undo*/
void MainWindow::on_toolButton_clicked()
{
    display->getElement()->undoAction();
    //qDebug() << "undoStack: " << display->getElement()->undoStack.size() << " " << display->getElement()->redoStack.size();

    if((display->getElement()->undoStack.size())==0){
        ui->toolButton->setDisabled(true);
    }
    else{
        if((display->getElement()->redoStack.size())!=0)
            ui->toolButton_2->setDisabled(false);
        ui->toolButton->setDisabled(false);
        }

    display->setImageInLabel();
}

/*redo*/
void MainWindow::on_toolButton_2_clicked()
{
    display->getElement()->redoAction();
    //qDebug() << "redoStack: " << display->getElement()->redoStack.size() << " " << display->getElement()->undoStack.size();;

    if(display->getElement()->redoStack.size()==0){
        ui->toolButton_2->setDisabled(true);
    }
    else{
        if((display->getElement()->undoStack.size())!=0)
            ui->toolButton->setDisabled(false);
        ui->toolButton_2->setDisabled(false);
    }

    display->setImageInLabel();
}

/*Prati događaje miša koji se dešavaju kada se pređe preko labele sa slikom.*/
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == display->getLabel() && cropPressed!=false){
        if(event->type()== QEvent::MouseButtonPress){
            QMouseEvent *newEvent = static_cast<QMouseEvent*>(event);

            /*Pamtimo gornje levo teme pravougaonika.*/
            startPoint = newEvent->pos()/* / scaleFactor*/;
            qDebug() << startPoint;

            rubberBandCreated = true;
            setCursor(Qt::CrossCursor);

            rubberBand = new QRubberBand(QRubberBand::Rectangle, display->getLabel());
            rubberBand->setGeometry(QRect(startPoint, QSize()));
            rubberBand->show();
        }
        else if(event->type() == QEvent::MouseMove){
                QMouseEvent *newEvent = static_cast<QMouseEvent*>(event);
                setCursor(Qt::CrossCursor);
                if(rubberBandCreated){
                    rubberBand->setGeometry(QRect(startPoint, newEvent->pos()));
                    rubberBand->show();
                }
            }
        else if(event->type() == QEvent::MouseButtonRelease){
                display->getElement()->saveAction();
                QMouseEvent *newEvent = static_cast<QMouseEvent*>(event);
                setCursor(Qt::ArrowCursor);

                /*Pamtimo gornje levo teme pravougaonika.*/
                endPoint = newEvent->pos()/* / scaleFactor*/;

                if(rubberBandCreated)
                    rubberBand->deleteLater();

                /*Sečemo selektovani deo.*/
                display->getElement()->cropImage(startPoint, endPoint);
                display->setImageInLabel();

                rubberBandCreated = false;
                cropPressed = false;

            }
            return true;
        }
    return false;
}

/*crop*/
void MainWindow::on_toolButton_5_clicked()
{
    display->getElement()->saveAction();
    emit enableUndoSignal();
    cropPressed = true;
    setCursor(Qt::ArrowCursor);
}
