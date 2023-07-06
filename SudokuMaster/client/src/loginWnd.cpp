#include <QMessageBox>
#include <QPushButton>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include "GlobalLogger.h"
#include "ui_loginWnd.h"
#include "loginWnd.h"
#include "sslSocketThread.h"
#include "sslSocketMessage.h"
#include "toolFunction.h"

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
        GlobalLogger::getInstance().log(GlobalLogger::MessageType::Info, "login in Btn cliked");
        if (ui->userNameEdit->text().isEmpty() || ui->passwordEdit->text().isEmpty()) {
            QMessageBox mes(this);
            mes.setText(tr("Username or password cannot be empty!"));
            mes.exec();
        } else {
            QJsonObject jsonObject;
            jsonObject.insert(sslSocketMessage::keyRequestType,
                              sslSocketMessage::RequestTypeQStr[sslSocketMessage::RequestType::LoginIn]);
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
            jsonObject.insert(sslSocketMessage::keyRequestType,
                              sslSocketMessage::RequestTypeQStr[sslSocketMessage::RequestType::SignUp]);
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
            &LoginWnd::processReceivedData);
}

void LoginWnd::processReceivedData(const QJsonObject &jsonObject) {
    if (!jsonObject.isEmpty()) {
        auto responseType = sslSocketMessage::QStrResponseType[jsonObject.value(
                sslSocketMessage::keyResponseType).toString()];
        QMessageBox messageBox(this);
        switch (responseType) {
            case sslSocketMessage::ResponseType::SignUpSuccess :
                messageBox.setText(tr("Sign up success, please login in"));
                break;
            case sslSocketMessage::ResponseType::UserNotExist :
                messageBox.setText(tr("User does not exist, please re-enter or sign up"));
                break;
            case sslSocketMessage::ResponseType::WrongPassword :
                messageBox.setText(tr("Wrong password, please re-enter"));
                break;
            case sslSocketMessage::ResponseType::UsernameAlreadyExists :
                messageBox.setText(tr("Username already exists, please re-enter"));
                break;
            case sslSocketMessage::ResponseType::LoginInSuccess:
                this->close();
                saveUserLoginInformation(jsonObject);
                break;
        }
        if (!messageBox.text().isEmpty()) {
            messageBox.exec();
        }
    } else {
        log(GlobalLogger::MessageType::Error, "The login window received Json Object is empty");
    }
}

void LoginWnd::log(const GlobalLogger::MessageType type, const QString &message) {
    GlobalLogger::getInstance().log(type, message);
}

void LoginWnd::saveUserLoginInformation(const QJsonObject &jsonObject) {
    QJsonObject userCache;
    auto iterUsername = jsonObject.constFind(
            sslSocketMessage::UserInfoTypeQStr[sslSocketMessage::UserInfoType::Username]);
    auto iterPassword = jsonObject.constFind(
            sslSocketMessage::UserInfoTypeQStr[sslSocketMessage::UserInfoType::Password]);
    userCache.insert(iterUsername.key(), iterUsername.value());
    userCache.insert(iterPassword.key(), iterPassword.value());

    // Cache user login information
    QString cacheFolderPath = "./userInfoCache";
    QDir cacheFolder(cacheFolderPath);
    if (!cacheFolder.exists()) {
        if (!cacheFolder.mkpath(".")) {
            log(GlobalLogger::MessageType::Error, QString("Failed to create <userInfoCache> folder."));
            return;
        }
    }
    QJsonDocument userJsonDocment(userCache);
    QString fileName(iterUsername.value().toString() + "%1");
    QFile encryptedFile(cacheFolderPath + "/" + fileName.arg(".user"));
    if (encryptedFile.open(QIODevice::WriteOnly)) {
        QByteArray key = "0123456789abcdef0123456789abcdef";
        QByteArray iv = "fedcba9876543210fedcba9876543210";
        QByteArray data = toolFunction::encrypt(key, iv, userJsonDocment.toJson());
        encryptedFile.write(data);
        encryptedFile.close();
    } else {
        log(GlobalLogger::MessageType::Error, QString("Failed to open or create %1").arg(encryptedFile.fileName()));
    }
}
