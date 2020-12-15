#ifndef DISPLAYAREA_H
#define DISPLAYAREA_H

#include "headers/image.h"

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QPixmap>
#include <iostream>
#include <vector>
#include <iterator>
#include <list>

class DisplayArea : public QWidget
{
    Q_OBJECT

public:
    explicit DisplayArea(QWidget *parent = nullptr);
    QLabel* getLabel();
    Image* getElement();
    void setElements(const QStringList& filePaths);
    void getNextElement();
    void getPreviousElement();
    size_t getSize();
    void setToBeginning();

    void freeImages();



    void setImageInLabel(QImage img);
    void setImageInLabel();

    void scaleImage(double factor);

public:
    QLabel* m_label; //zbog smestanja privremene kopije TODO napraviti ovaj metod da ne bi bilo public

public:
    std::vector<Image*> m_elements;
    std::vector<Image*>::iterator m_current;

//signals:

};

#endif // DISPLAYAREA_H
