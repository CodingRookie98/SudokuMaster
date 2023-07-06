#ifndef LOGINWND_H
#define LOGINWND_H

#include <QDialog>
#include "GlobalLogger.h"

namespace Ui {
    class LoginWnd;
}

class LoginWnd : public QDialog {
Q_OBJECT

public:
    explicit LoginWnd(QWidget *parent = nullptr);

    ~LoginWnd() override;

private:
    Ui::LoginWnd *ui;

    void init();

    void signalProcess();

    void BtnEvent();

    void loginInClicked();

    void signUpClicked();

    static QString sha256(const QString &password);

    void processReceivedData(const QJsonObject& jsonObject);

    static void log(GlobalLogger::MessageType type, const QString& message);

    void saveUserLoginInformation(const QJsonObject& jsonObject);
};

#endif // LOGINWND_H
