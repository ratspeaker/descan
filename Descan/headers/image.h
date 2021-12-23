#ifndef IMAGE_H
#define IMAGE_H

#include <stack>
#include <map>
#include <cmath>
#include <QWidget>
#include <QImage>
#include <QString>
#include <QDebug>
#include <QPixmap>
#include <QPdfWriter>
#include <QPainter>

class Image
{

public:
    Image(const QString& filePath);
    void loadImage(QString filePath);

    QImage getImage();
    void setImage(QImage img);

    QString getFilePath();
    void setFilePath(QString filePath);

    QSize size();
    bool isNull();

    double getScaleFactor();
    void setScaleFactor(double factor);

    std::map<QString, int> getSlider();
    void setSlider(QString str, int position);

    QImage resizeImage(double factor, char option);
    QImage changeBrightness(double brightnessFactor);
    QImage changeContrast(double contrastFactor);
    QImage gammaCorrection(double gamma);
    QImage greyScale();
    QImage changeSaturation(double saturationChange);

    int width();
    int height();

    void printImageIntoPdf(QPainter &painter);

    void saveAction();
    void undoAction();
    void redoAction();

    void cropImage(QPoint startPoint, QPoint endPoint);
    void rotateImage(int angle);
    void emptyUndoActions();
    void emptyRedoActions();

public:
    std::stack<std::pair<QImage, std::map<QString, int>>> undoStack;
    std::stack<std::pair<QImage, std::map<QString, int>>> redoStack;

    std::map<QString, int> sliders = {{"scale", 0}, {"hor", 0}, {"ver", 0},
                                      {"brigh", 0}, {"con", 0}, {"gam", 0}, {"sat", 0}};

private:
    QImage m_image;
    QString m_filePath = "";
    double scaleFactor = 0.0;
};

double truncate(double x);

#endif // IMAGE_H
