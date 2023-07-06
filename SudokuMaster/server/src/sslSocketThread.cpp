/**
  ******************************************************************************
  * @file           : sslSocketThread.cpp
  * @author         : hui
  * @brief          : None
  * @attention      : None
  * @date           : 2023/6/30
  ******************************************************************************
  */

#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlQuery>
#include <QSqlError>
#include "sslSocketThread.h"
#include "sslSocketMessage.h"
#include "databaseOperation.h"

SslSocketThread::SslSocketThread(const QSharedPointer<QSslSocket> &sslSocket)
        : QRunnable(), sharedSslSocket(sslSocket) {

    setAutoDelete(false);
}

SslSocketThread::~SslSocketThread() {
    sharedSslSocket.reset();
}

void SslSocketThread::run() {
    init();
    signalProcess();
}

void SslSocketThread::init() {
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("sudoku_master_test");
    db.setUserName("sudokumaster_server");
    db.setPassword("SudokuMasterServer");
    db.setPort(3306);
    if (!db.open()) {
        qDebug() << "Failed to open database";
        qDebug() << db.lastError().text();
        qDebug() << "Available drivers:";
        QStringList drivers = QSqlDatabase::drivers();
                foreach(QString driver, drivers) {
                qDebug() << driver;
            }
    } else {
        qDebug() << "opened database";
    }
}

void SslSocketThread::signalProcess() {
    connect(sharedSslSocket.get(), &QSslSocket::readyRead, this, &SslSocketThread::processReadyReadData);
}

void SslSocketThread::log(const QString &message) {
    qDebug() << message;
}

void SslSocketThread::processReadyReadData() {
    auto *data = new QByteArray(sharedSslSocket->readAll());
    auto *jsonDocument = new QJsonDocument(QJsonDocument::fromJson(*data));
    if (!jsonDocument->isNull()) {
        if (jsonDocument->isObject()) {
            QJsonObject object = jsonDocument->object();
            sslSocketMessage::RequestType requestType =
                    sslSocketMessage::QStrRequestType[object.value(sslSocketMessage::keyRequestType).toString()];
            switch (requestType) {
                case sslSocketMessage::RequestType::SignUp :
                    signUpRequestProcess(object);
                    break;
                case sslSocketMessage::LoginIn:
                    loginInRequestProcess(object);
                    break;
            }
        } else {

        }
    } else {

    }
    delete data;
    delete jsonDocument;
}

void SslSocketThread::signUpRequestProcess(const QJsonObject &jsonObject) {
    QJsonObject res;
    auto const iter = jsonObject.find(sslSocketMessage::keyRequestType);
    QString username = jsonObject.value(
            sslSocketMessage::UserInfoTypeQStr[sslSocketMessage::UserInfoType::Username]).toString();
    QString password = jsonObject.value(
            sslSocketMessage::UserInfoTypeQStr[sslSocketMessage::UserInfoType::Password]).toString();
    QString sql = databaseOperation::DatabaseOperationQStr[databaseOperation::DatabaseOperationType::add_user].arg(
            username, password);
    res.insert(iter.key(), iter.value());

    QSqlQuery query(db);
    if (query.exec(sql)) {
        if (query.next()) {
            bool success = query.value(0).toBool();
            if (success) {
                res.insert(sslSocketMessage::keyResponseType,
                           sslSocketMessage::ResponseTypeQStr[sslSocketMessage::ResponseType::SignUpSuccess]);
            } else {
                sql = databaseOperation::DatabaseOperationQStr[databaseOperation::DatabaseOperationType::select_user_name].arg(
                        username);
                if (query.exec(sql)) {
                    if (query.next()) {
                        bool exist = query.value(0).toBool();
                        if (exist) {
                            res.insert(sslSocketMessage::keyRequestType,
                                       sslSocketMessage::ResponseTypeQStr[sslSocketMessage::ResponseType::UsernameAlreadyExists]);
                        } else {
                            qDebug() << QString(
                                    "The database does not exist %1, but inserting username: %2 , password: %3 failed").arg(
                                    username, password);
                        }
                    }
                } else {
                    qDebug() << "Failed to select user : " + username;
                }
            }
        }
    } else {
        qDebug() << query.lastError();
        qDebug() << "Failed to add user : " + username + " , " + password;
    }
    QJsonDocument jsonDocument(res);
    writeToUser(jsonDocument);
}

void SslSocketThread::writeToUser(const QJsonDocument &jsonDocument) {
    QByteArray byteArray = jsonDocument.toJson();
    if (sharedSslSocket->write(byteArray) <= 0) {
        qDebug() << "write data to user is failed";
    }
}

void SslSocketThread::loginInRequestProcess(const QJsonObject &jsonObject) {
    QJsonObject res;
    auto const iter = jsonObject.find(sslSocketMessage::keyRequestType);
    QString username = jsonObject.value(
            sslSocketMessage::UserInfoTypeQStr[sslSocketMessage::UserInfoType::Username]).toString();
    QString password = jsonObject.value(
            sslSocketMessage::UserInfoTypeQStr[sslSocketMessage::UserInfoType::Password]).toString();
    QString sql = databaseOperation::DatabaseOperationQStr[databaseOperation::DatabaseOperationType::select_user_name].arg(
            username);
    res.insert(iter.key(), iter.value());

    QSqlQuery query(db);
    // Query whether a user exists in the database
    if (query.exec(sql)) {
        if (query.next()) {
            bool success = query.value(0).toBool();
            // If the user exists
            if (success) {
                sql = databaseOperation::DatabaseOperationQStr[databaseOperation::DatabaseOperationType::is_correct_password].arg(
                        username, password);
                // Verify username and password
                if (query.exec(sql)) {
                    if (query.next()) {
                        bool ok = query.value(0).toBool();
                        if (ok) {
                            /*
                             * If the login is successful, the username and password hash value sent by the client will be
                             * sent back to the client so that the client can cache the user login information
                             */
                            res = jsonObject;
                            res.insert(sslSocketMessage::keyResponseType,
                                       sslSocketMessage::ResponseTypeQStr[sslSocketMessage::ResponseType::LoginInSuccess]);
                        } else {
                            res.insert(sslSocketMessage::keyResponseType,
                                       sslSocketMessage::ResponseTypeQStr[sslSocketMessage::ResponseType::WrongPassword]);
                        }
                    }
                } else {
                    qDebug() << query.lastError();
                    qDebug() << QString("Failed to execute function  is_correct_password(%1, %2)")
                            .arg(username, password);
                }
            } else {
                //If the user not exists
                res.insert(sslSocketMessage::keyResponseType,
                           sslSocketMessage::ResponseTypeQStr[sslSocketMessage::ResponseType::UserNotExist]);
            }
        }
    } else {
        qDebug() << query.lastError();
        qDebug() << QString("Failed to execute function select_user_name(%1)").arg(username);
    }
    QJsonDocument jsonDocument(res);
    writeToUser(jsonDocument);
}
