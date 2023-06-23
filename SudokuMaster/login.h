#ifndef LOGIN_H
#define LOGIN_H

#include "qmessagebox.h"
#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private:
    Ui::Login *ui;
    QMessageBox message;

    void init();
    void BtnEvent();
    QString sha256(QString password);
};

#endif // LOGIN_H
