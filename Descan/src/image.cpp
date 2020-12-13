#include "headers/image.h"
#include <cmath>

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

void Image::setScaleFactor(double factor)
{
    scaleFactor = factor;
}

void Image::setSlider(QString str, int position)
{
    sliders[str] = position;
}

std::map<QString, int> Image::getSlider()
{
    return sliders;
}

/*Funkcija koja u zavisnosti od opcije menja i/ili visinu/duzinu slike i vraca izmenjen objekat*/
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

/*Funkcija koja menja svetlost same slike */
/*TODO: 4 funkcije na isti nacin rade, promeniti tako da se pozivaju iste funkcije samo sa odgovarajucim parametrima */
QImage Image::changeBrightness(double brightnessFactor)
{
    QImage newImage(m_image.width(),m_image.height(), QImage::Format_ARGB32);
    double newRed, newBlue, newGreen;
    /*Prolazi kroz svaki piksel i povecava ga za odredjen faktor */
    for (int x = 0; x < m_image.width(); ++x)
        for (int y = 0; y < m_image.height(); ++y) {
            QRgb color =m_image.pixel(x, y);
            newRed = truncate(qRed(color) + brightnessFactor);
            newGreen = truncate(qGreen(color) + brightnessFactor);
            newBlue = truncate(qBlue(color) + brightnessFactor);
            newImage.setPixel(x, y, qRgb(newRed,newGreen,newBlue));
        }

   return newImage;
}

/*Funkcija koja menja kontrast slike i vraca izmenjen objekat */
QImage Image::changeContrast(double contrastFactor)
{
    QImage newImage(m_image.width(),m_image.height(), QImage::Format_ARGB32);
    double contrastCorrection = static_cast<double>(259.0*(255+contrastFactor)/(255.0*(259-contrastFactor)));
    double newRed, newBlue, newGreen;
    for (int x = 0; x < m_image.width(); ++x)
        for (int y = 0; y < m_image.height(); ++y) {
            QRgb color = m_image.pixel(x, y);
            newRed = truncate(contrastCorrection*(qRed(color)-128)+128);
            newBlue = truncate(contrastCorrection*(qBlue(color)-128)+128);
            newGreen = truncate(contrastCorrection*(qGreen(color)-128)+128);
            newImage.setPixel(x, y, qRgb(newRed,newGreen,newBlue));
        }
    return newImage;
}

/*Gama korekcija za sliku, svaki piksel se zamenjuje novim pikselom koji se racuna po formuli */
QImage Image::gammaCorrection(double gamma)
{
    QImage newImage(m_image.width(),m_image.height(), QImage::Format_ARGB32);
    double gammaCorrection = 1/gamma;
    double newRed, newBlue, newGreen;
    for (int x = 0; x < m_image.width(); ++x)
        for (int y = 0; y < m_image.height(); ++y) {
            QRgb color = m_image.pixel(x, y);
            newRed = 255 * pow(qRed(color)/ 255.0,gammaCorrection);
            newBlue = 255 * pow(qBlue(color)/255.0,gammaCorrection);
            newGreen = 255 * pow(qGreen(color)/ 255.0,gammaCorrection);
            newImage.setPixel(x, y, qRgb(newRed,newGreen,newBlue));
        }
    return newImage;
}

/*Funkcija koja vraca crno belu sliku */
QImage Image::greyScale()
{
    QImage newImage(m_image.width(),m_image.height(), QImage::Format_ARGB32);
    double value;
    for (int x = 0; x < m_image.width(); ++x)
        for (int y = 0; y < m_image.height(); ++y) {
            QRgb color = m_image.pixel(x, y);
            /*lepo namesteni tezinski faktori za racunanje odgovarajuce nijanse sive *
             *alternativa je da se uzme aritmeticka sredina vrednosti RGB boja piksela */
            value = (0.299) * qRed(color) + (0.587) * qGreen(color) + (0.114)*qBlue(color);
            newImage.setPixel(x, y, qRgb(value,value,value));
        }
    return newImage;
}


QImage Image::changeSaturation(double saturationChange)
{
    QImage newImage(m_image.width(),m_image.height(), QImage::Format_ARGB32);
    double newRed, newBlue, newGreen;
    for (int x = 0; x < m_image.width(); ++x)
        for (int y = 0; y < m_image.height(); ++y) {
            QRgb color = m_image.pixel(x, y);
            double pixelFactor = sqrt(qRed(color)*qRed(color)*0.299 +qGreen(color)*qGreen(color)*0.587+qBlue(color)*qBlue(color)*0.114);
            newRed = truncate(pixelFactor + (qRed(color) - pixelFactor)*saturationChange);
            newGreen = truncate(pixelFactor + (qGreen(color) - pixelFactor)*saturationChange);
            newBlue = truncate(pixelFactor + (qBlue(color) - pixelFactor)*saturationChange);
            newImage.setPixel(x, y, qRgb(newRed,newGreen,newBlue));
        }
    return newImage;
}
/*Geteri za visinu i sirinu slike*/
int Image::width()
{
    return m_image.width();
}

int Image::height() {
    return m_image.height();
}
/*Cuva se na steku slika, za potrebe undo opcije*/
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
/*Rotira se slika za odredjeni ugao*/
void Image::rotateImage(int angle)
{
    QPixmap pixmap = QPixmap::fromImage(m_image);
    QTransform mat;
    mat.rotate(angle);
    pixmap = pixmap.transformed(mat);
    m_image = pixmap.toImage();
}

//videti sta sa ovim
/*Pomocna funkcija koja vodi racuna da vrednost piksela bude u intervalu [-255,255]*/
double truncate(double x) {
    if (x > 255)
        return 255;
    if (x < 0)
        return 0;
    return x;
}

double Image::getScaleFactor()
{
    return scaleFactor;
}
