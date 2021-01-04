#include "headers/displayarea.h"

DisplayArea::DisplayArea(QWidget *parent) : QWidget(parent)
{
    m_label = new QLabel();
}

DisplayArea::~DisplayArea()
{
    for (auto &element: m_elements) {
        delete element;
    }
    delete m_label;
}

QLabel* DisplayArea::getLabel()
{
    return m_label;
}

//vraca trenutnu sliku
Image* DisplayArea::getElement()
{
    return *m_current;
}

//vraca vektor objekata Image*
std::vector<Image*> DisplayArea::getElements() {
    return m_elements;
}

//popunjava vektor objekata Image*
void DisplayArea::setElements(const QStringList &filePaths)
{
    for (auto i=filePaths.cbegin(); i != filePaths.cend(); i++) {
        m_elements.push_back(new Image(*i));
    }
    for (auto i=m_elements.begin(); i != m_elements.end(); i++) {
        (*i)->setScaleFactor(1.0);
    }
    setToBeginning();
}

//postavlja se iterator na pocetak
void DisplayArea::setToBeginning()
{
    m_current = m_elements.begin();
}

//postavlja se iterator na naredni element
void DisplayArea::getNextElement()
{
    if (m_current != m_elements.cend()-1)
    {
        if (m_current == m_elements.cend()-2) {
            indDisable = 2;
        }
        m_current++;
    }
}

//postavlja se iterator na prethodni element
void DisplayArea::getPreviousElement()
{
    if (m_current != m_elements.cbegin())
    {
        if (m_current == m_elements.cbegin()+1) {
            indDisable = 1;
        }
        m_current--;
    }
}

//indikator za disable dugmica
int DisplayArea::getIndDisable() {
    return indDisable;
}

//vraca velicinu vektora slika
size_t DisplayArea::getSize()
{
    return m_elements.size();
}

//cisti vektor slika
void DisplayArea::freeImages()
{
    m_elements.clear();
}

//postavlja sliku u labelu
void DisplayArea::setImageInLabel(QImage img)
{
    (*m_current)->setImage(img);
    m_label->setPixmap(QPixmap::fromImage((*m_current)->getImage()));
    scaleImage(1.0);
}

void DisplayArea::setImageInLabel()
{
    m_label->setPixmap(QPixmap::fromImage((*m_current)->getImage()));
    scaleImage(1.0);
}

//funkcija za potrebe zumiranja
void DisplayArea::scaleImage(double factor)
{
    double newScaleFactor = (*m_current)->getScaleFactor() * factor;
    (*m_current)->setScaleFactor(newScaleFactor);

    QPixmap pixmap = QPixmap::fromImage((*m_current)->getImage());
    pixmap = pixmap.scaled(newScaleFactor*pixmap.size());

    m_label->setPixmap(pixmap);
    m_label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_label->resize(pixmap.size().width(), pixmap.size().height());
}
