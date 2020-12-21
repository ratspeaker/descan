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
    std::vector<Image*> getElements();
    void setElements(const QStringList &filePaths);

    void setToBeginning();
    void getNextElement();
    void getPreviousElement();
    int getIndDisable();

    size_t getSize();
    void freeImages();
    void setImageInLabel(QImage img);
    void setImageInLabel();
    void scaleImage(double factor);

public:
    QLabel* m_label; //zbog smestanja privremene kopije
                     //TODO napraviti ovaj metod da ne bi bilo public
    std::vector<Image*> m_elements;
private:

    std::vector<Image*>::iterator m_current;
    int indDisable = 0;
};

#endif // DISPLAYAREA_H
