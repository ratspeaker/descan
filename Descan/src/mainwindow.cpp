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
    display->getLabel()->setMouseTracking(true);  //za
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


void MainWindow::on_toolButton_5_clicked()
{

}

void MainWindow::on_toolButton_6_clicked()
{

}

// TODO: treba izmeniti da ne menja direktno sliku(kopija objekta ili naci bolje resenje?)
void MainWindow::on_hsScale_2_sliderMoved(int position)
{
    double resizeFactor = 0.4;
    if(position)
        resizeFactor = 0.4 + (2-0.4)*position/100;

    image_copy = display->getElement()->resizeImage(resizeFactor, 's');
    display->m_label->setPixmap(QPixmap::fromImage(image_copy));
}

void MainWindow::on_hsHorizontal_2_sliderMoved(int position)
{
    double resizeFactor = 0.4;
    if(position)
        resizeFactor = 0.4 + (2-0.4)*position/100;

    image_copy = display->getElement()->resizeImage(resizeFactor, 'w');
    display->m_label->setPixmap(QPixmap::fromImage(image_copy));
}

void MainWindow::on_hsVertical_2_sliderMoved(int position)
{
    double resizeFactor = 0.4;
    if(position)
        resizeFactor = 0.4 + (2-0.4)*position/100;

    image_copy = display->getElement()->resizeImage(resizeFactor, 'h');
    display->m_label->setPixmap(QPixmap::fromImage(image_copy));
}



void MainWindow::on_pbGreyscale_clicked()
{
    image_copy = display->getElement()->greyScale();
    display->setImageInLabel(image_copy);
}

void MainWindow::on_hsBrightness_sliderMoved(int position)
{
    double brightnessFactor = (((position * (100 - (-100)))) / 100) -100;
    image_copy = display->getElement()->changeBrightness(brightnessFactor);
    display->m_label->setPixmap(QPixmap::fromImage(image_copy)); //prikazuje se samo privremena kopija
}

void MainWindow::on_hsBrightness_sliderReleased()
{
     display->setImageInLabel(image_copy);
}



void MainWindow::on_hsContrast_sliderMoved(int position)
{
    double contrastFactor = (((position * (100 - (-100)))) / 100) -100;
    image_copy = display->getElement()->changeContrast(contrastFactor);
    display->m_label->setPixmap(QPixmap::fromImage(image_copy)); //prikazuje se samo privremena kopija
}



void MainWindow::on_hsContrast_sliderReleased()
{
    display->setImageInLabel(image_copy);
}

void MainWindow::on_hsCorrection_sliderMoved(int position)
{
    image_copy = display->getElement()->gammaCorrection(position*0.02);
    display->m_label->setPixmap(QPixmap::fromImage(image_copy)); //prikazuje se samo privremena kopija
}

void MainWindow::on_hsCorrection_sliderReleased()
{
     display->setImageInLabel(image_copy);
}

void MainWindow::on_hsSaturation_sliderMoved(int position)
{
    image_copy = display->getElement()->changeSaturation(position*0.02);
    display->m_label->setPixmap(QPixmap::fromImage(image_copy)); //prikazuje se samo privremena kopija
}

void MainWindow::on_hsSaturation_sliderReleased()
{
    display->setImageInLabel(image_copy);
}
