#include "headers/pdfhandler.h"

PDFHandler::PDFHandler(QWidget *parent) : QWidget(parent)
{
    PDFNet::Initialize();
}

PDFHandler::~PDFHandler()
{
    qDebug() << "cao";
    PDFNet::Terminate();
}

void PDFHandler::convertImagesIntoPdf(QString& filename, std::vector<Image*> imageElements)
{
    //instanciranje objekta za pravljenje pdfa i neka njegova podesavanja
    QPdfWriter pdfWriter(filename);
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

void PDFHandler::mergePdf()
{
    if (!inputFilesMerge.isEmpty()) {
        QString outputFile = QFileDialog::getSaveFileName(this, tr("Save"), "/home/", tr("*.pdf"));

        if (!outputFile.isEmpty()) {

            qDebug() << "Spajanje više PDF-ova u jedan";
            PDFDoc newDocument;
            newDocument.InitSecurityHandler();

            for (auto i = 0; i < inputFilesMerge.size(); i++) {
                QString inputFile(inputFilesMerge[i]);
                PDFDoc inDocument(inputFile.toStdString());
                newDocument.InsertPages(i, inDocument, 1, inDocument.GetPageCount(), PDFDoc::e_none);
             }

             newDocument.Save(outputFile.toStdString(), SDFDoc::e_remove_unused, 0);

             QMessageBox::information(this, tr("Merge PDF"),
                                            tr("Your files have been successfully merged!"),
                                            QMessageBox::Cancel | QMessageBox::Ok,
                                            QMessageBox::Cancel);
        }
    }
}

void PDFHandler::splitPdf()
{
    if (!inputFileSplit.isEmpty()) {
        int index = inputFileSplit.lastIndexOf("/");
        int size = inputFileSplit.length() - index - 4;
        QString fileName = inputFileSplit.mid(index, size);

        PDFDoc inputDocument(inputFileSplit.toStdString());
        inputDocument.InitSecurityHandler();
        int pageCount = inputDocument.GetPageCount();

        if (pageCount == 1) {
            QMessageBox::warning(this, tr("Split PDF"),
                                       tr("Can't split single page document"),
                                       QMessageBox::Cancel | QMessageBox::Ok,
                                       QMessageBox::Cancel);
        } else {
            bool ok;
            QString input = QInputDialog::getText(this, tr("Split PDF"),
                                                  tr("Split on pages:"), QLineEdit::Normal, "", &ok, Qt::Dialog);

            vector<int> pages;
            if (ok && !input.isEmpty()) {
                QStringList list(input.split(","));

                for (auto page = list.cbegin(); page != list.cend(); page++)
                {
                    int indx = (*page).toInt();

                    if (indx > 0 && indx <= pageCount) {
                        pages.push_back(indx);
                    } else {
                        QMessageBox::warning(this, tr("Split PDF"),
                                                   tr("Entered page number out of range"),
                                                   QMessageBox::Cancel | QMessageBox::Ok,
                                                   QMessageBox::Cancel);
                    }

                }

                pages.push_back(pageCount);
            }


             QString outputPath = QFileDialog::getExistingDirectory(this, tr("Save"), "/home/", QFileDialog::ShowDirsOnly);

             int i = 1;
             if (!outputPath.isEmpty()) {
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
                     newDocument.InsertPages(0, inputDocument, prev, (*it), PDFDoc::e_none);
                     newDocument.Save(outputFile.toStdString(), SDFDoc::e_remove_unused, 0);
                     i++;
                   }

                   QMessageBox::information(this, tr("Split PDF"),
                                                  tr("Your file has been successfully split!"),
                                                  QMessageBox::Cancel | QMessageBox::Ok,
                                                  QMessageBox::Cancel);
             }
        }
    }
}

void PDFHandler::setInputFilesMerge(const QStringList &fileName)
{
    inputFilesMerge = fileName;
}

void PDFHandler::setInputFileSplit(const QString &fileName)
{
    inputFileSplit = fileName;
}
