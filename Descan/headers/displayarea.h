#ifndef DISPLAYAREA_H
#define DISPLAYAREA_H

#include "headers/image.h"

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QPixmap>

class DisplayArea : public QWidget
{
    Q_OBJECT

public:
    explicit DisplayArea(QWidget *parent = nullptr);
    QLabel* getLabel();
    Image* getElement();
    void setElement(const QString& filePath);
    void setImageInLabel(QImage img);
    void setImageInLabel();
    double getScaleFactor();
    void setScaleFactor(double factor);
    void scaleImage(double factor);

public:
    QLabel* m_label; //zbog smestanja privremene kopije TODO napraviti ovaj metod da ne bi bilo public

private:
    Image* m_element;
    double scaleFactor;

//signals:

};

#endif // DISPLAYAREA_H
