
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QSslServer>
#include "LogServer.h"

class sslServer : public QSslServer
{
    Q_OBJECT
public:
    explicit sslServer(QObject *parent = nullptr);

private:
    void init();
    void signalProcess();
    void log(const LogServer::MessageType& type, const QString& text);
};

#endif // TCPSERVER_H
