/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QStackedWidget *stackedWidget;
    QWidget *main;
    QLabel *lblDescan;
    QPushButton *pbNextEdit;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QPushButton *pbImportMultiple;
    QPushButton *pbImport;
    QPushButton *pbSplitPdf;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbMergePdf;
    QWidget *editImg;
    QWidget *page;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(950, 709);
        MainWindow->setMaximumSize(QSize(1200, 850));
        stackedWidget = new QStackedWidget(MainWindow);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(-20, 20, 1021, 681));
        stackedWidget->setLayoutDirection(Qt::RightToLeft);
        main = new QWidget();
        main->setObjectName(QString::fromUtf8("main"));
        lblDescan = new QLabel(main);
        lblDescan->setObjectName(QString::fromUtf8("lblDescan"));
        lblDescan->setGeometry(QRect(150, 40, 591, 151));
        lblDescan->setPixmap(QPixmap(QString::fromUtf8(":/img/1.png")));
        pbNextEdit = new QPushButton(main);
        pbNextEdit->setObjectName(QString::fromUtf8("pbNextEdit"));
        pbNextEdit->setGeometry(QRect(660, 610, 161, 51));
        QFont font;
        font.setFamily(QString::fromUtf8("Ubuntu Condensed"));
        font.setPointSize(14);
        font.setItalic(true);
        pbNextEdit->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8("next.png"), QSize(), QIcon::Selected, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/img/next.png"), QSize(), QIcon::Selected, QIcon::On);
        pbNextEdit->setIcon(icon);
        pbNextEdit->setIconSize(QSize(20, 20));
        widget = new QWidget(main);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(151, 330, 671, 231));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        pbImportMultiple = new QPushButton(widget);
        pbImportMultiple->setObjectName(QString::fromUtf8("pbImportMultiple"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(15);
        sizePolicy.setHeightForWidth(pbImportMultiple->sizePolicy().hasHeightForWidth());
        pbImportMultiple->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Ubuntu Condensed"));
        font1.setPointSize(16);
        font1.setItalic(true);
        pbImportMultiple->setFont(font1);
        pbImportMultiple->setIconSize(QSize(30, 30));

        gridLayout_2->addWidget(pbImportMultiple, 1, 2, 1, 1);

        pbImport = new QPushButton(widget);
        pbImport->setObjectName(QString::fromUtf8("pbImport"));
        pbImport->setFont(font1);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img/import4.png"), QSize(), QIcon::Selected, QIcon::On);
        pbImport->setIcon(icon1);
        pbImport->setIconSize(QSize(30, 30));

        gridLayout_2->addWidget(pbImport, 0, 2, 1, 1);

        pbSplitPdf = new QPushButton(widget);
        pbSplitPdf->setObjectName(QString::fromUtf8("pbSplitPdf"));
        pbSplitPdf->setFont(font1);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/img/split.png"), QSize(), QIcon::Selected, QIcon::On);
        pbSplitPdf->setIcon(icon2);
        pbSplitPdf->setIconSize(QSize(30, 30));

        gridLayout_2->addWidget(pbSplitPdf, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 1, 1, 1);

        pbMergePdf = new QPushButton(widget);
        pbMergePdf->setObjectName(QString::fromUtf8("pbMergePdf"));
        pbMergePdf->setFont(font1);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/img/file.png"), QSize(), QIcon::Selected, QIcon::On);
        pbMergePdf->setIcon(icon3);
        pbMergePdf->setIconSize(QSize(30, 30));

        gridLayout_2->addWidget(pbMergePdf, 1, 0, 1, 1);

        stackedWidget->addWidget(main);
        editImg = new QWidget();
        editImg->setObjectName(QString::fromUtf8("editImg"));
        stackedWidget->addWidget(editImg);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        stackedWidget->addWidget(page);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        lblDescan->setText(QString());
        pbNextEdit->setText(QCoreApplication::translate("MainWindow", "Next   ", nullptr));
        pbImportMultiple->setText(QCoreApplication::translate("MainWindow", "Choose multiple images...", nullptr));
        pbImport->setText(QCoreApplication::translate("MainWindow", "Import image    ", nullptr));
        pbSplitPdf->setText(QCoreApplication::translate("MainWindow", " Split PDF   ", nullptr));
        pbMergePdf->setText(QCoreApplication::translate("MainWindow", "Merge PDF   ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
