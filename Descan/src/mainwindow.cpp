#include "headers/mainwindow.h"
#include "headers/image.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

/*
//otvara se dijalog za odabir slika i prikazuje se u labeli
void MainWindow::on_pbImport_clicked()
{
    m_img->m_filePath = QFileDialog::getOpenFileName(this,tr("Import image"),"/home/",tr("Images (*.png *.xpm *.jpg *.bmp)"));
    qDebug() << m_img->m_filePath;

    cv::Mat image = m_img->LoadImage();
    m_img->ScaleImage(image);
    QPixmap pixmap = m_img->DisplayImage(image);
    ui->labelImage->setPixmap(pixmap);
}

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

void MainWindow::on_pbImport_clicked()
{

}
void MainWindow::on_pbImportMultiple_clicked()
{

}

void MainWindow::on_hsBrightness_3_sliderMoved(int position)
{

}

void MainWindow::on_toolButton_5_clicked()
{

}

void MainWindow::on_toolButton_6_clicked()
{

}


void MainWindow::on_hsScale_2_sliderMoved(int position)
{
    double resizeFactor = 0.33;
    if(position)
        resizeFactor = 0.33 + (3-0.33)*position/100;

    img->resizeImage(resizeFactor, 's');
}

void MainWindow::on_hsHorizontal_2_sliderMoved(int position)
{
    double resizeFactor = 0.33;
    if(position)
        resizeFactor = 0.33 + (3-0.33)*position/100;

    img->resizeImage(resizeFactor, 'w');
}

void MainWindow::on_hsVertical_2_sliderMoved(int position)
{
    double resizeFactor = 0.33;
    if(position)
        resizeFactor = 0.33 + (3-0.33)*position/100;

    img->resizeImage(resizeFactor, 'h');
}

