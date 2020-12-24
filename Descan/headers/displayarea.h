#ifndef DISPLAYAREA_H
#define DISPLAYAREA_H

#include "headers/image.h"

#include <iostream>
#include <vector>
#include <iterator>
#include <QWidget>
#include <QLabel>
#include <QList>
#include <QPixmap>

class DisplayArea : public QWidget
{
    Q_OBJECT

public:
    explicit DisplayArea(QWidget *parent = nullptr);
    ~DisplayArea();

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
    QLabel* m_label;
    std::vector<Image*> m_elements;

private:
    std::vector<Image*>::iterator m_current;
    int indDisable = 0;
};

#endif // DISPLAYAREA_H
