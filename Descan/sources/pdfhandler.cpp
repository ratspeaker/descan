#include "headers/pdfhandler.h"

PDFHandler::PDFHandler(QWidget *parent) : QWidget(parent)
{
    PDFNet::Initialize();
}

PDFHandler::~PDFHandler()
{
    PDFNet::Terminate();
}

void PDFHandler::convertImagesIntoPdf(QString &fileName, std::vector<Image*> &imageElements)
{
    //instanciranje objekta za pravljenje pdfa i neka njegova podesavanja
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setResolution(150);
    pdfWriter.setPageMargins(QMargins(20, 20, 20, 30));
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));

    //instanciranje objekta Painter koji ce stampati slike u ovaj pdf fajl
    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);

    //prolazimo kroz svaku sliku u vektoru
    unsigned i=0;
    for (auto& image: imageElements) {
        //uzimamo trenutnu sliku
        image->printImageIntoPdf(painter);

        //stampa se watermark
        painter.drawText(QPoint(0, painter.device()->height() + 30), "Descan");

        //nakon poslednje se ne stampa nova strana
        if (i != (imageElements.size()-1)) {
            pdfWriter.newPage();
        }
        i++;
    }
}

void PDFHandler::compressPDF(QString& fileName)
{
    PDFDoc doc(fileName.toStdString().c_str());
    doc.InitSecurityHandler();
    Optimizer::Optimize(doc);
    doc.Save(fileName.toStdString().c_str(), SDFDoc::e_linearized, NULL);
}

QString PDFHandler::mergePdf()
{
    QString outputFile = QFileDialog::getSaveFileName(this, tr("Save"), "/home/", tr("*.pdf"));
    QString output = outputFile + ".pdf";

    if (!outputFile.isEmpty()) {
        qDebug() << "Spajanje više PDF-ova u jedan";
        PDFDoc newDocument;
        newDocument.InitSecurityHandler();

        int index = 0;
        for (auto i = 0; i < inputFilesMerge.size(); i++) {
            QString inputFile(inputFilesMerge[i]);
            PDFDoc inDocument(inputFile.toStdString().c_str());

            //qDebug() << index;
            newDocument.InsertPages(index, inDocument, 1, inDocument.GetPageCount(), PDFDoc::e_none);
            index += (inDocument.GetPageCount() + 1);
        }

        newDocument.Save(output.toStdString().c_str(), SDFDoc::e_remove_unused, 0);

        QMessageBox::information(this, tr("Merge PDF"), tr("Your files have been successfully merged!"));
    }

    return output;
}

QStringList PDFHandler::splitPdf()
{
    QStringList pdfFiles;
    int index = inputFileSplit.lastIndexOf("/");
    int size = inputFileSplit.length() - index - 4;
    QString fileName = inputFileSplit.mid(index, size);

    //provera da li je fajl ispravan
    try {
        PDFDoc inputDocument(inputFileSplit.toStdString().c_str());
        inputDocument.InitSecurityHandler();
        int pageCount = inputDocument.GetPageCount();

        //ako dokument ima samo jednu stranicu
        if (pageCount == 1) {
            QMessageBox::warning(this, tr("Split PDF"),
                                       tr("Can't split single page document."),
                                       QMessageBox::Cancel | QMessageBox::Ok,
                                       QMessageBox::Cancel);
            return pdfFiles;
        }

        bool ok = false;
        QString input;
        while (!ok) {
            input = QInputDialog::getText(this, tr("Split PDF"),
                                                tr("Split on pages:"),
                                                QLineEdit::Normal, "", &ok,
                                                Qt::Dialog);
        }

        //ako korisnik nije uneo broj stranice
        if (input.isEmpty()) {
            QMessageBox::warning(this, tr("Split PDF"),
                                       tr("You have to enter page number."),
                                       QMessageBox::Cancel | QMessageBox::Ok,
                                       QMessageBox::Cancel);
            return pdfFiles;
        }

        vector<int> pages;
        QStringList list(input.split(","));
        for (auto page = list.cbegin(); page != list.cend(); page++) {
            int indx = (*page).toInt();
            if (indx > 0 && indx <= pageCount) {
                pages.push_back(indx);
            }
            else {
                //ako broj stranice nije validan
                QMessageBox::warning(this, tr("Split PDF"),
                                           tr("Entered page number out of range."),
                                           QMessageBox::Cancel | QMessageBox::Ok,
                                           QMessageBox::Cancel);
                return pdfFiles;
            }
        }
        pages.push_back(pageCount);

        QString username = QString::fromStdString(getlogin());
        QString outputPath = QFileDialog::getExistingDirectory(this, tr("Save"), "/home/"+username, QFileDialog::ShowDirsOnly);
        qDebug() << outputPath;

        int i = 1;
        //if (!outputPath.isEmpty()) {
            int prev = 1;
            for (auto it = pages.cbegin(); it != pages.cend(); it++)
            {
                if (it != pages.cbegin()) {
                    if ((*it) == pageCount && std::next(it) == pages.cend() && (*std::prev(it)) == (*it)) {
                        prev = pageCount;
                    } else {
                        prev = (*std::prev(it))+1;
                    }
                }

                PDFDoc newDocument;
                QString outputFileName(QString("%1_%2.pdf").arg(fileName).arg(i));
                QString outputFile(outputPath + outputFileName);
                pdfFiles.append(outputFile);
                newDocument.InsertPages(0, inputDocument, prev, (*it), PDFDoc::e_none);
                newDocument.Save(outputFile.toStdString(), SDFDoc::e_remove_unused, 0);
                i++;
            }
            QMessageBox::information(this, tr("Split PDF"), tr("Your file has been successfully split!"));
        //}
    }
    catch(Common::Exception& e)
    {
        qDebug() << "Exception";
        QMessageBox::information(this, tr("Descan"), tr("Cannot load document."));
    }
    catch(...)
    {
        qDebug() << "Unknown Exception";
        QMessageBox::information(this, tr("Descan"), tr("Cannot load document."));
    }

    return pdfFiles;
}

void PDFHandler::setInputFilesMerge(const QStringList &fileName)
{
    inputFilesMerge = fileName;
}

void PDFHandler::setInputFileSplit(const QString &fileName)
{
    inputFileSplit = fileName;
}
