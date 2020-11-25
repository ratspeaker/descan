#include "headers/mainwindow.h"
#include<opencv4/opencv2/highgui.hpp>
#include<opencv4/opencv2/core.hpp>
#include<opencv4/opencv2/imgproc.hpp>
#include<opencv4/opencv2/imgcodecs.hpp>
#include<QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}
