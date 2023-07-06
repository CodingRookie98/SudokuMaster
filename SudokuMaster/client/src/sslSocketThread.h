//
// Created by hui on 2023/6/26.
//

#ifndef SUDOKUMASTER_SSLSOCKETTHREAD_H
#define SUDOKUMASTER_SSLSOCKETTHREAD_H

#include <QSslSocket>
#include "GlobalLogger.h"
#include <QJsonObject>
#include <QJsonDocument>

class SslSocketThread : public QSslSocket, public QRunnable {
Q_OBJECT

public:
    static SslSocketThread &getInstance();

    SslSocketThread(const SslSocketThread &) = delete;

    SslSocketThread &operator=(const SslSocketThread &) = delete;

    ~SslSocketThread() override;

signals:

    void readyReadForLoginWnd(const QJsonObject jsonObject);

    void readyWrite(const QJsonDocument jsonDoc);

protected:
    void run() override;

private:

    explicit SslSocketThread(QObject *parent = nullptr);

    void init();

    void signalProcess();

    static void log(const GlobalLogger::MessageType &type, const QString &text);

    // Distribute received data to different windows or threads
    void distributeData();

    void writeToServer(const QJsonDocument& jsonDocument);
};


#endif //SUDOKUMASTER_SSLSOCKETTHREAD_H
