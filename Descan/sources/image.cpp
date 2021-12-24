#include "headers/image.h"

Image::Image(const QString& filePath)
{
    m_filePath = filePath;
    loadImage(filePath);
}

void Image::loadImage(QString filePath)
{
    m_image.load(filePath);
}

QImage Image::getImage()
{
    return m_image;
}

void Image::setImage(QImage img)
{
    m_image = img;
}

QString Image::getFilePath()
{
    return m_filePath;
}

void Image::setFilePath(QString filePath)
{
    m_filePath = filePath;
}

QSize Image::size()
{
    return m_image.size();
}

bool Image::isNull()
{
    return m_image.isNull();
}

double Image::getScaleFactor()
{
    return scaleFactor;
}

void Image::setScaleFactor(double factor)
{
    scaleFactor = factor;
}

std::map<QString, int> Image::getSlider()
{
    return sliders;
}

void Image::setSlider(QString str, int position)
{
    sliders[str] = position;
}

//funkcija koja u zavisnosti od opcije menja visinu i/ili sirinu i vraca izmenjen objekat
QImage Image::resizeImage(double factor, char option)
{
    QImage newImage(m_image);

    double newWidth = m_image.width();
    double newHeight = m_image.height();

    if (option == 'w') {
        newWidth *= factor;
    } else if (option == 'h') {
        newHeight *= factor;
    } else if (option == 's') {
        newWidth *= factor;
        newHeight *= factor;
    }

    QSize newSize(newWidth, newHeight);
    return newImage.scaled(newSize, Qt::IgnoreAspectRatio);
}

//funkcija koja menja osvetljenje slike i vraca izmenjen objekat
//zbog efikasnosti izbacena je funkcija pixel i koriscena je
//funkcija scanLine koja vraca pokazivac na prvi piksel u liniji
QImage Image::changeBrightness(double brightnessFactor)
{
    QImage newImage(m_image.width(), m_image.height(), QImage::Format_ARGB32);
    double newRed, newBlue, newGreen;
    QRgb* rgb;
    //prolazi kroz svaki piksel i povecava ga za odredjen faktor
    for (int y = 0; y < m_image.height(); ++y) {
        rgb = (QRgb*)m_image.scanLine(y);
        for (int x = 0; x < m_image.width(); ++x) {
            newRed = truncate(qRed(rgb[x]) + brightnessFactor);
            newGreen = truncate(qGreen(rgb[x]) + brightnessFactor);
            newBlue = truncate(qBlue(rgb[x]) + brightnessFactor);
            //*rgb = qRgb(newRed, newGreen, newBlue); //hocemo izmene na novoj slici
            newImage.setPixel(x, y, qRgb(newRed, newGreen, newBlue));
        }
    }
    return newImage;
}

//funkcija koja menja kontrast slike i vraca izmenjen objekat
QImage Image::changeContrast(double contrastFactor)
{
    QImage newImage(m_image.width(), m_image.height(), QImage::Format_ARGB32);
    double contrastCorrection = static_cast<double>(259.0*(255+contrastFactor)/(255.0*(259-contrastFactor)));
    QRgb* rgb;
    double newRed, newBlue, newGreen;
    for (int y = 0; y < m_image.height(); ++y) {
        rgb = (QRgb*)m_image.scanLine(y);
        for (int x = 0; x < m_image.width(); ++x) {
            newRed = truncate(contrastCorrection*(qRed(rgb[x])-128)+128);
            newBlue = truncate(contrastCorrection*(qBlue(rgb[x])-128)+128);
            newGreen = truncate(contrastCorrection*(qGreen(rgb[x])-128)+128);
            newImage.setPixel(x, y, qRgb(newRed, newGreen, newBlue));
        }
    }
    return newImage;
}

//gama korekcija za sliku, svaki piksel se zamenjuje novim pikselom koji se racuna po formuli
QImage Image::gammaCorrection(double gamma)
{
    QImage newImage(m_image.width(), m_image.height(), QImage::Format_ARGB32);
    double gammaCorrection = 1/gamma;
    QRgb* rgb;
    double newRed, newBlue, newGreen;
    for (int y = 0; y < m_image.height(); ++y) {
        rgb = (QRgb*)m_image.scanLine(y);
        for (int x = 0; x < m_image.width(); ++x) {
            newRed = 255 * pow(qRed(rgb[x])/ 255.0,gammaCorrection);
            newBlue = 255 * pow(qBlue(rgb[x])/255.0,gammaCorrection);
            newGreen = 255 * pow(qGreen(rgb[x])/ 255.0,gammaCorrection);
            newImage.setPixel(x, y, qRgb(newRed, newGreen, newBlue));
        }
    }
    return newImage;
}

//funkcija koja vraca crno belu sliku
QImage Image::greyScale()
{
    QImage newImage(m_image.width(), m_image.height(), QImage::Format_ARGB32);
    double value;
    QRgb* rgb;
    for (int y = 0; y < m_image.height(); ++y) {
        rgb = (QRgb*)m_image.scanLine(y);
        for (int x = 0; x < m_image.width(); ++x) {
            //lepo namesteni tezinski faktori za racunanje odgovarajuce nijanse sive
            //alternativa je da se uzme aritmeticka sredina vrednosti RGB boja piksela
            value = (0.299) * qRed(rgb[x]) + (0.587) * qGreen(rgb[x]) + (0.114)*qBlue(rgb[x]);
            newImage.setPixel(x, y, qRgb(value, value, value));
        }
    }
    return newImage;
}

QImage Image::changeSaturation(double saturationChange)
{
    QImage newImage(m_image.width(), m_image.height(), QImage::Format_ARGB32);
    double newRed, newBlue, newGreen;
    QRgb* rgb;
    for (int y = 0; y < m_image.height(); ++y) {
        rgb = (QRgb*)m_image.scanLine(y);
        for (int x = 0; x < m_image.width(); ++x) {
            double pixelFactor = sqrt(qRed(rgb[x])*qRed(rgb[x])*0.299 +qGreen(rgb[x])*qGreen(rgb[x])*0.587+qBlue(rgb[x])*qBlue(rgb[x])*0.114);
            newRed = truncate(pixelFactor + (qRed(rgb[x]) - pixelFactor)*saturationChange);
            newGreen = truncate(pixelFactor + (qGreen(rgb[x]) - pixelFactor)*saturationChange);
            newBlue = truncate(pixelFactor + (qBlue(rgb[x]) - pixelFactor)*saturationChange);
            newImage.setPixel(x, y, qRgb(newRed, newGreen, newBlue));
        }
    }
    return newImage;
}

//geteri za sirinu i visinu slike
int Image::width()
{
    return m_image.width();
}

int Image::height() {
    return m_image.height();
}

//funkcija koja stampa sliku u pdf dokument
void Image::printImageIntoPdf(QPainter &painter)
{
    double img_width  = static_cast<double>(m_image.width());
    double img_height = static_cast<double>(m_image.height());
    //qDebug() << "visina" << img_height << "sirina" << img_width;

    QRect source(0, 0, img_width, img_height);
    QPixmap imgPixmap = QPixmap::fromImage(m_image);

    //ako je slika manja od papira
    if (img_width <= painter.device()->width() && img_height <= painter.device()->height()) {
        painter.drawPixmap(source, imgPixmap);
    }
    //ako je veca od papira, da je ne sece
    else {
        double imageRatio = img_height / img_width;

        double scaleBy;
        if (painter.device()->width() * imageRatio > painter.device()->height()) {
            scaleBy = painter.device()->height() / img_height;
        }
        else {
            scaleBy = painter.device()->width() / img_width;
        }

        QRect target(0, 0, img_width * scaleBy, img_height * scaleBy);
        painter.drawPixmap(target, imgPixmap, source);
    }
}

//cuva se na steku slika, za potrebe undo opcije
void Image::saveAction()
{
    undoStack.push({m_image, sliders});
}

void Image::undoAction()
{
    redoStack.push({m_image, sliders});
    auto tmp = undoStack.top();;
    m_image = tmp.first;
    sliders = tmp.second;
    //qDebug() << sliders;
    undoStack.pop();
}

void Image::redoAction()
{
    undoStack.push({m_image, sliders});
    auto tmp = redoStack.top();;
    m_image = tmp.first;
    sliders = tmp.second;
    redoStack.pop();
}

void Image::cropImage(QPoint startPoint, QPoint endPoint)
{
    QRect rect = QRect(startPoint, endPoint);
    m_image = m_image.copy(rect);
}

void Image::rotateImage(int angle)
{
    QPixmap pixmap = QPixmap::fromImage(m_image);
    QTransform mat;
    mat.rotate(angle);
    pixmap = pixmap.transformed(mat);
    m_image = pixmap.toImage();
}

void Image::emptyUndoActions()
{
    while(!undoStack.empty()) {
        undoStack.pop();
    }
}

void Image::emptyRedoActions()
{
    while(!redoStack.empty()) {
        redoStack.pop();
    }
}

//pomocna funkcija koja vodi racuna da vrednost piksela bude u intervalu [-255, 255]
double truncate(double x) {
    if (x > 255)
        return 255;
    if (x < 0)
        return 0;
    return x;
}
