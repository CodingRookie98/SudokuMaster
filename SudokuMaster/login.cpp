#include "login.h"
#include "qdebug.h"
#include "qpushbutton.h"
#include "ui_login.h"
#include <QCryptographicHash>

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
    message.setParent(this);
}

void Login::BtnEvent()
{
    connect(ui->loginInBtn, &QPushButton::clicked, this, [&](){
//        qDebug() << "login in Btn cliked";
        if(ui->userNameEdit->text().isEmpty() || ui->passwordEdit->text().isEmpty()) {
            message.setText("Username or password cannot be empty!");
            message.exec();
        }else {
            QString userName(ui->userNameEdit->text());
            QString password(sha256(ui->passwordEdit->text()));
            qDebug() << userName << ": " << password << password.size();
        }
    });

     connect(ui->signUpBtn, &QPushButton::clicked, this, [&](){
        qDebug() << "sign up Btn cliked";
        if(ui->userNameEdit->text().isEmpty() || ui->passwordEdit->text().isEmpty()) {
            message.setText("Username or password cannot be empty!");
            message.exec();
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
