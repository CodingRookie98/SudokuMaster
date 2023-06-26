#include "login.h"
#include "qdebug.h"
#include "qmessagebox.h"
#include "qpushbutton.h"
#include "ui_login.h"
#include <QCryptographicHash>
#include "DatabaseSingleton.h"
#include <QSqlQuery>
#include "LogServer.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    init();
    BtnEvent();
}

Login::~Login()
{
    delete ui;
}

void Login::init()
{
    ui->loginInBtn->setText(tr("Login in"));
    ui->signUpBtn->setText(tr("Sign up"));
    ui->userNameLabel->setText(tr("User Name"));
    ui->passwordLabel->setText(tr("PassWord"));
    ui->rememberMe->setText(tr("Remember me"));
}

void Login::BtnEvent()
{
    loginInClicked();
    signUpClicked();
}

void Login::loginInClicked() {
    connect(ui->loginInBtn, &QPushButton::clicked, this, [&]() {
        //        qDebug() << "login in Btn cliked";
        if (ui->userNameEdit->text().isEmpty() || ui->passwordEdit->text().isEmpty()) {
            QMessageBox mes(this);
            mes.setText(tr("Username or password cannot be empty!"));
            mes.exec();
        } else {
            QString userName(ui->userNameEdit->text());
            QString password(sha256(ui->passwordEdit->text()));
            qDebug() << userName << ": " << password << password.size();
        }
    });
}

void Login::signUpClicked() {
    connect(ui->signUpBtn, &QPushButton::clicked, this, [&]() {
        if (ui->userNameEdit->text().isEmpty() || ui->passwordEdit->text().isEmpty()) {
            QMessageBox mes(this);
            mes.setText(tr("Username or password cannot be empty!"));
            mes.exec();
        } else {
            QString userName = ui->userNameEdit->text();
            QString password = sha256(ui->passwordEdit->text());

            QSqlQuery query(DatabaseSingleton::getInstance());
            QString select = "SELECT userName FROM user_info WHERE userName = :userName";
            query.prepare(select);
            query.bindValue(":userName", userName);

            if (query.exec()) {
                if (query.next()) {
                    QMessageBox mes(this);
                    mes.setText(QString("Username \"%1\" already taken").arg(userName));
                    mes.exec();
                } else {
                    QString insert = "INSERT INTO user_info (userName, pwd) VALUES (:userName, :password)";
                    query.prepare(insert);
                    query.bindValue(":userName", userName);
                    query.bindValue(":password", password);

                    if (query.exec()) {
                        LogServer(LogServer::MessageType::_INFO, insert);
                        QMessageBox mes(this);
                        mes.setText("Sign up success, please login");
                        mes.exec();
                    } else {
                        LogServer(LogServer::MessageType::_ERROR, insert);
                        qDebug() << "query.exec() failed";
                    }
                }
            } else {
                LogServer(LogServer::MessageType::_ERROR, select);
                qDebug() << "query.exec(select) failed!!!!";
            }
        }
    });
}

QString Login::sha256(QString password)
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(password.toUtf8());
    // 计算哈希值
    QByteArray hashResult = hash.result();
    // 将哈希值转换为十六进制字符串
    QString hashString = hashResult.toHex();
    return hashString;
}
