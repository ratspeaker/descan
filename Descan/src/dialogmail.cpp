#include "headers/dialogmail.h"
#include "ui_dialogmail.h"
#include <QFileDialog>

DialogMail::DialogMail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMail)
{
    ui->setupUi(this);
    this->setWindowTitle("Send Mail");
}

DialogMail::~DialogMail()
{
    delete ui;
}

void DialogMail::on_pbExit_clicked()
{
    close();
}

void DialogMail::on_pbBrowse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Document"), "/home/");
    ui->leAttach->setText(fileName);
}
