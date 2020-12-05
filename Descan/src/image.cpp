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

QString Image::getFilePath()
{
    return m_filePath;
}

void Image::setImage(QImage img)
{
    m_image = img;
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

void Image::resizeImage(double factor, char option)
{
    double newWidth = m_image.size().width();
    double newHeight = m_image.size().height();

    if (option == 'w') {
        newWidth *= factor;
    } else if (option == 'h') {
        newHeight *= factor;
    } else if (option == 's') {
        newWidth *= factor;
        newHeight *= factor;
    }

    QSize newSize(newWidth, newHeight);
    m_image = m_image.scaled(newSize, Qt::IgnoreAspectRatio);
}
