#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

#include<QAction>
#include<QMessageBox>
#include<QMenu>
#include<QMenuBar>
#include<QFileDialog>
#include<filesystem>
#include<QtAlgorithms>
#include<QLabel>
#include<algorithm>
#include<QDebug>
#include <QTimer>
#include<QImage>
#include<QGraphicsScene>
#include<QGraphicsPixmapItem>
#include<QGraphicsItem>
#include<QPixmap>
#include"headers/image.hpp"
#include<opencv4/opencv2/highgui/highgui.hpp>
#include<opencv4/opencv2/imgproc/imgproc.hpp>
#include<opencv4/opencv2/imgcodecs/imgcodecs.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow), m_img(new Image())
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

void MainWindow::on_pbBack1_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::DisplayImage(QString& fileName) {
//ucitavamo sliku
cv::Mat img = cv::imread(fileName.toUtf8().constData());
// TODO srediti lepo ovaj double
double scale = float(581)/img.size().width;

//otvara se dijalog za odabir slika i prikazuje se u labeli
void MainWindow::on_pbImport_clicked()
{
    m_img->m_filePath = QFileDialog::getOpenFileName(this,tr("Import image"),"/home/",tr("Images (*.png *.xpm *.jpg *.bmp)"));
    qDebug() << m_filePath;

    QPixmap pixmap = m_img->DisplayImage();

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
