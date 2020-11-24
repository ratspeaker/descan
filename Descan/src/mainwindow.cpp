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

#include<opencv4/opencv2/highgui/highgui.hpp>
#include<opencv4/opencv2/imgproc/imgproc.hpp>
#include<opencv4/opencv2/imgcodecs/imgcodecs.hpp>

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

void MainWindow::on_pbBack1_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::DisplayImage(QString& fileName) {
//ucitavamo sliku
cv::Mat img = cv::imread(fileName.toUtf8().constData());
// TODO srediti lepo ovaj double
double scale = float(581)/img.size().width;

//Skaliramo sliku na odgovarajucu velicinu labele
cv::resize(img,img,cv::Size(0,0),scale,scale);

//kastovanje u tip slike koji se koristi u Qt
QImage imdisplay((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
//Hendlujemo problem kad je slika veca od prostora

/*I nacin --> scroll bar */
//QGraphicsItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(imdisplay));
//QGraphicsScene* scene = new QGraphicsScene(this);
//ui->graphicsView->setScene(scene);
//scene->addItem(item);

/*II nacin --> Skaliranje */
ui->labelImage->setPixmap(QPixmap::fromImage(imdisplay));
}

// Otvara se dijalog za odabir slika
void MainWindow::on_pbImport_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Import image"),"/home/",tr("Images (*.png *.xpm *.jpg *.bmp)"));
    qDebug() << fileName;
    DisplayImage(fileName);

}
