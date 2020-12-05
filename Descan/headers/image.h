#ifndef IMAGE_H
#define IMAGE_H

#include <QWidget>
#include <QImage>
#include <QString>
#include <QDebug>

class Image : public QWidget
{
    Q_OBJECT

public:
    explicit Image(QWidget *parent = nullptr);

    void loadImage(QString filePath);
    QImage getImage();
    QString getFilePath();
    void setImage(QImage img);
    void setFilePath(QString filePath);
    QSize size();
    bool isNull();
    void resizeImage(double factor, char option);

private:
    QImage m_image;
    QString m_filePath;

//signals:

};

#endif // IMAGE_H
