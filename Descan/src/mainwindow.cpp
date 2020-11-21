#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

#include<QAction>
#include<QMessageBox>
#include<QMenu>
#include<QMenuBar>

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
