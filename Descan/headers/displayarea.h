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
    double getScaleFactor();
    void setElement(const QString& filePath);
    void setScaleFactor(double factor);
    void setImageInLabel(QImage img);
    void setImageInLabel();
    QPixmap scaleImage(double scaleFactor);

    QLabel* m_label; //zbog smestanja privremene kopije TODO napraviti ovaj metod da ne bi bilo public
private:

    Image* m_element;
    double scaleFactor;
//signals:

};

#endif // DISPLAYAREA_H
