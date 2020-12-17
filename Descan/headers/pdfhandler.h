#ifndef PDFHANDLER_H
#define PDFHANDLER_H

#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QDebug>
#include <QPdfWriter>
#include <QPainter>
#include "image.h"

#include "../PDFNetC64/Headers/PDF/PDFNet.h"
#include "../PDFNetC64/Headers/PDF/PDFDoc.h"
#include "../PDFNetC64/Headers/PDF/PDFDocInfo.h"
#include "../PDFNetC64/Headers/SDF/SDFDoc.h"

using namespace std;
using namespace pdftron;
using namespace SDF;
using namespace PDF;

class PDFHandler  : public QWidget
{
    Q_OBJECT

public:
    PDFHandler(QWidget *parent = nullptr);
    ~PDFHandler();

    void mergePdf();
    void splitPdf();

    void setInputFilesMerge(const QStringList &fileName);
    void setInputFileSplit(const QString &fileName);

    static void convertImagesIntoPdf(QString &filename, std::vector<Image*> imageElements);

private:
    QStringList inputFilesMerge;
    QString inputFileSplit;
};

#endif // PDFHANDLER_H
