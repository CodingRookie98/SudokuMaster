
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QSslServer>
#include "LogServer.h"
#include <QObject>

class sslServer : public QSslServer
{
    Q_OBJECT
public:
    explicit sslServer(QObject *parent = nullptr);

private:
    void init();
    void signalProcess();
    static void log(const LogServer::MessageType& type, const QString& text);
public slots:
    static void errorProcess(const QSslSocket* socket, const QAbstractSocket::SocketError& error);
};

#endif // TCPSERVER_H
