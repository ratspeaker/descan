#include "headers/displayarea.h"

DisplayArea::DisplayArea(QWidget *parent) : QWidget(parent)
{
    m_label = new QLabel();
}

QPixmap DisplayArea::scaleImage(double scaleFactor)
{
    QPixmap pixmap = QPixmap::fromImage(m_element->getImage());
    qDebug() << "pixmapa" << pixmap.size().width() << " " << pixmap.size().height(); //ovo je ok

    pixmap = pixmap.scaled(scaleFactor*pixmap.size());
    qDebug() << "skalirana pixmapa" << pixmap.size().width() << " " << pixmap.size().height(); //ovo je ok
    qDebug() << m_element->getImage().size().width() << " " << m_element->getImage().size().height();   //ovo je ok
    return pixmap;
}

QLabel *DisplayArea::getLabel()
{
    return m_label;
}

double DisplayArea::getScaleFactor()
{
    return scaleFactor;
}

void DisplayArea::setElement(const QString &filePath)
{
    m_element = new Image(filePath);
}

void DisplayArea::setScaleFactor(double factor)
{
    scaleFactor = factor;
}

Image *DisplayArea::getElement()
{
    return m_element;
}

void DisplayArea::setImageInLabel(QImage img)
{
    m_element->setImage(img);
    m_label->setPixmap(QPixmap::fromImage(m_element->getImage()));
}

void DisplayArea::setImageInLabel()
{
    m_label->setPixmap(QPixmap::fromImage(m_element->getImage()));
}
