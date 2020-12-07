#include "headers/displayarea.h"

DisplayArea::DisplayArea(QWidget *parent) : QWidget(parent)
{
    m_label = new QLabel();
}

void DisplayArea::scaleImage(double scaleFactor)
{
    QPixmap pixmap = QPixmap::fromImage(m_element->getImage());
    pixmap = pixmap.scaled(scaleFactor*pixmap.size());
    m_label->setPixmap(pixmap);
    qDebug() << pixmap.size().width() << " " << pixmap.size().height();
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
