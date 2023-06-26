//
// Created by hui on 2023/6/26.
//

#ifndef SUDOKUMASTER_TCPSERVER_H
#define SUDOKUMASTER_TCPSERVER_H

#include <QTcpSocket>

class tcpService : public QTcpSocket {
Q_OBJECT

public:
    enum RequestType {
        SignUp,
        LoginIn
    };

    tcpService(QObject *parent);

    void writeToServer(const RequestType &type, const QByteArray &data);

private:
    void readFromServer();

signals:

//    void dataFromServerReady(const tcpService::RequestType &type, const QByteArray &data);
};


#endif //SUDOKUMASTER_TCPSERVER_H
