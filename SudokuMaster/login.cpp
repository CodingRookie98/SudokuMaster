#include "login.h"
#include "qdebug.h"
#include "qmessagebox.h"
#include "qpushbutton.h"
#include "ui_login.h"
#include <QCryptographicHash>
#include "DatabaseSingleton.h"
#include <QSqlQuery>

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
    //    message.setParent(this);
}

void Login::BtnEvent()
{
    connect(ui->loginInBtn, &QPushButton::clicked, this, [&]() {
        qDebug() << "login in Btn cliked";
        if (ui->userNameEdit->text().isEmpty() || ui->passwordEdit->text().isEmpty()) {
            //            message.setText(tr("Username or password cannot be empty!"));
            //            message.exec();
            QMessageBox mes(this);
            mes.setText(tr("Username or password cannot be empty!"));
            mes.exec();
        } else {
            QString userName(ui->userNameEdit->text());
            QString password(sha256(ui->passwordEdit->text()));
            qDebug() << userName << ": " << password << password.size();
        }
    });

    connect(ui->signUpBtn, &QPushButton::clicked, this, [&]() {
        qDebug() << "sign up Btn cliked";
        if (ui->userNameEdit->text().isEmpty() || ui->passwordEdit->text().isEmpty()) {
            QMessageBox mes(this);
            mes.setText(tr("Username or password cannot be empty!"));
            mes.exec();
        } else {
            QString userName(ui->userNameEdit->text());
            QSqlQuery query(DatabaseSingleton::getInstance());
            QString select = QString("SELECT userName FROM user_info WHERE userName = '%1'").arg(userName);
            if (query.prepare(select)) {
                bool res = query.exec();
                // * 如果不存在同样的用户名便向数据库插入数据
                if (res && !query.lastQuery().isEmpty()) {
                    qDebug() << "can insert";
                    QString password(sha256(ui->passwordEdit->text()));
                    QString insert = QString("INSERT INTO user_info (userName, pwd) VALUES ('%1','%2')").arg(userName).arg(password);
                    if (query.prepare(insert)) {
                        if (!query.exec()) {
                            qDebug() << "query.exec() failed";
                        } else {
                            QMessageBox mes(this);
                            mes.setText("sign up sucess, please login in");
                            mes.exec();
                        }
                    } else {
                        qDebug() << "query.prepare() failed";
                    }
                } else if (res && query.lastQuery().isEmpty()) {
                    QMessageBox mes(this);
                    mes.setText(QString("username \"%1\" already taken").arg(userName));
                    mes.exec();
                } else if (!res) {
                    qDebug() << "query.exec(select) failed";
                }
            } else {
                qDebug() << (query.isValid() ? "query is valid" : "query not valid");
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
