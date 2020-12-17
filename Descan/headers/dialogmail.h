#ifndef DIALOGMAIL_H
#define DIALOGMAIL_H

#include <QDialog>

namespace Ui {
class DialogMail;
}

class DialogMail : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMail(QWidget *parent = nullptr);
    ~DialogMail();

private slots:
    void on_pbExit_clicked();

    void on_pbBrowse_clicked();

private:
    Ui::DialogMail *ui;
};

#endif // DIALOGMAIL_H
