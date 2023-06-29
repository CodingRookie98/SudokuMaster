//
// Created by hui on 2023/6/26.
//

#ifndef SUDOKUMASTER_TCPSERVER_H
#define SUDOKUMASTER_TCPSERVER_H

#include <QSslSocket>
#include "LogServer.h"

class sslSocket : public QSslSocket {
Q_OBJECT

public:
    enum RequestType {
        SignUp,
        LoginIn
    };

    explicit sslSocket(QObject *parent = nullptr);

private:

    void init();
    void signalProcess();
    static void log(const LogServer::MessageType& type, const QString& text);

signals:

};


#endif //SUDOKUMASTER_TCPSERVER_H
