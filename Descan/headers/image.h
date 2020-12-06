#ifndef IMAGE_H
#define IMAGE_H

#include <QWidget>
#include <QImage>
#include <QString>
#include <QDebug>
#include <QPixmap>

class Image
{

public:
    Image(const QString& filePath);
    void loadImage(QString filePath);
    QImage getImage();
    QString getFilePath();
    void setImage(QImage img);
    void setFilePath(QString filePath);
    QSize size();
    bool isNull();
    void resizeImage(double factor, char option);

    QImage changeBrightness(double brightnessFactor);
    QImage changeContrast(double contrastFactor);
    QImage gammaCorrection(double gamma);
    QImage greyScale();
    QImage changeSaturation(double saturationChange);

    int width();
    int height();

private:
    QImage m_image;
    QString m_filePath ;

//signals:

};

double truncate(double x);

#endif // IMAGE_H
