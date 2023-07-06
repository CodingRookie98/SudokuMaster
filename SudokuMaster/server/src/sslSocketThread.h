/**
  ******************************************************************************
  * @file           : sslSocketThread.h
  * @author         : hui
  * @brief          : None
  * @attention      : None
  * @date           : 2023/6/30
  ******************************************************************************
  */

#ifndef SUDOKUMASTER_SSLSOCKETTHREAD_H
#define SUDOKUMASTER_SSLSOCKETTHREAD_H

#include <QSslSocket>
#include <QRunnable>
#include <QJsonObject>
#include <QSqlDatabase>

class SslSocketThread : public QObject, public QRunnable {
Q_OBJECT

public:
    explicit SslSocketThread(const QSharedPointer<QSslSocket> &sslSocket);

    ~SslSocketThread() override;

signals:

protected:
    void run() override;

private:
    QSharedPointer<QSslSocket> sharedSslSocket;
    QSqlDatabase db;

    void init();

    void signalProcess();

    void log(const QString& message) ;

    void processReadyReadData();

    void signUpRequestProcess(const QJsonObject& jsonObject);

    void loginInRequestProcess(const QJsonObject& jsonObject);

    void writeToUser(const QJsonDocument& jsonDocument);
};


#endif //SUDOKUMASTER_SSLSOCKETTHREAD_H
