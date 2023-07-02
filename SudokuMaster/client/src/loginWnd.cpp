#include <QMessageBox>
#include <QPushButton>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include "GlobalLogger.h"
#include "ui_loginWnd.h"
#include "loginWnd.h"
#include "sslSocketThread.h"
#include "sslSocketMessage.h"

LoginWnd::LoginWnd(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::LoginWnd) {
    ui->setupUi(this);
    init();
    signalProcess();
}

LoginWnd::~LoginWnd() {
    delete ui;
}

void LoginWnd::init() {
    ui->loginInBtn->setText(tr("Login in"));
    ui->signUpBtn->setText(tr("Sign up"));
    ui->userNameLabel->setText(tr("User Name"));
    ui->passwordLabel->setText(tr("PassWord"));
    ui->rememberMe->setText(tr("Remember me"));
}

void LoginWnd::BtnEvent() {
    loginInClicked();
    signUpClicked();
}

void LoginWnd::loginInClicked() {
    connect(ui->loginInBtn, &QPushButton::clicked, this, [&]() {
        //        qDebug() << "login in Btn cliked";
        GlobalLogger::getInstance().log(GlobalLogger::MessageType::Info, "login in Btn cliked");
        if (ui->userNameEdit->text().isEmpty() || ui->passwordEdit->text().isEmpty()) {
            QMessageBox mes(this);
            qDebug() << "Login!!";
            mes.setText(tr("Username or password cannot be empty!"));
            mes.exec();
        } else {
            QJsonObject jsonObject;
            jsonObject.insert(sslSocketMessage::MessageOwnerQStr[sslSocketMessage::MessageOwner::LoginWnd],
                              sslSocketMessage::MessageOwnerQStr[sslSocketMessage::MessageOwner::LoginWnd]);
            jsonObject.insert(sslSocketMessage::ClientMessageTypeQStr[sslSocketMessage::ClientMessageType::LoginIn],
                              sslSocketMessage::ClientMessageTypeQStr[sslSocketMessage::ClientMessageType::LoginIn]);
            jsonObject.insert(sslSocketMessage::UserInfoTypeQStr[sslSocketMessage::UserInfoType::Username],
                              ui->userNameEdit->text());
            jsonObject.insert(sslSocketMessage::UserInfoTypeQStr[sslSocketMessage::UserInfoType::Password],
                              sha256(ui->passwordEdit->text()));
            QJsonDocument jsonDocument(jsonObject);
            emit SslSocketThread::getInstance().readyWrite(jsonDocument);
        }
    });
}

void LoginWnd::signUpClicked() {
    connect(ui->signUpBtn, &QPushButton::clicked, this, [&]() {
        if (ui->userNameEdit->text().isEmpty() || ui->passwordEdit->text().isEmpty()) {
            QMessageBox mes(this);
            mes.setText(tr("Username or password cannot be empty!"));
            mes.exec();
        } else {
            QJsonObject jsonObject;
            jsonObject.insert(sslSocketMessage::MessageOwnerQStr[sslSocketMessage::MessageOwner::LoginWnd],
                              sslSocketMessage::MessageOwnerQStr[sslSocketMessage::MessageOwner::LoginWnd]);
            jsonObject.insert(sslSocketMessage::ClientMessageTypeQStr[sslSocketMessage::ClientMessageType::SignUp],
                              sslSocketMessage::ClientMessageTypeQStr[sslSocketMessage::ClientMessageType::SignUp]);
            jsonObject.insert(sslSocketMessage::UserInfoTypeQStr[sslSocketMessage::UserInfoType::Username],
                              ui->userNameEdit->text());
            jsonObject.insert(sslSocketMessage::UserInfoTypeQStr[sslSocketMessage::UserInfoType::Password],
                              sha256(ui->passwordEdit->text()));
            QJsonDocument jsonDocument(jsonObject);
            emit SslSocketThread::getInstance().readyWrite(jsonDocument);
        }
    });
}

QString LoginWnd::sha256(const QString &password) {
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(password.toUtf8());
    // 计算哈希值
    QByteArray hashResult = hash.result();
    // 将哈希值转换为十六进制字符串
    QString hashString = hashResult.toHex();
    return hashString;
}

void LoginWnd::signalProcess() {
    BtnEvent();

    connect(&SslSocketThread::getInstance(), &SslSocketThread::readyReadForLoginWnd, this,
            &LoginWnd::processSslSocketMessage);
}

void LoginWnd::processSslSocketMessage(const QJsonDocument jsonDoc) {
    if (!jsonDoc.isNull()) {
        if (jsonDoc.isObject()) {
            QJsonObject object = jsonDoc.object();
            QString messageOwner = object[sslSocketMessage::keyMessageOwner].toString();
            if (sslSocketMessage::QStrMessageOwner[messageOwner] == sslSocketMessage::MessageOwner::LoginWnd) {
                for (QJsonObject::ConstIterator itr = object.constBegin(); itr != object.constEnd(); ++itr) {
                    sslSocketMessage::ServerMessageType messageType = sslSocketMessage::QStrServerMessageType[itr.value().toString()];
                    QMessageBox messageBox(this);
                    switch (messageType) {
                        case sslSocketMessage::ServerMessageType::SignUpSuccess :
                            messageBox.setText(tr("Sign up success, please login in"));
                            break;
                        case sslSocketMessage::ServerMessageType::UserNotExist :
                            messageBox.setText(tr("User does not exist, please re-enter or sign up"));
                            break;
                        case sslSocketMessage::ServerMessageType::WrongPassword :
                            messageBox.setText(tr("Wrong password, please re-enter"));
                            break;
                        case sslSocketMessage::ServerMessageType::UsernameAlreadyExists :
                            messageBox.setText(tr("Username already exists, please re-enter"));
                            break;
                    }
                    messageBox.exec();
                }
            }
        } else {
            log(GlobalLogger::MessageType::Error, "The SslSocket received Json Document is not a Object");
        }
    } else {
        log(GlobalLogger::MessageType::Error, "The SslSocket received Json Document is not null");
    }
}

void LoginWnd::log(const GlobalLogger::MessageType type, const QString &message) {
    GlobalLogger::getInstance().log(type, message);
}
