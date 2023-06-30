
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QSslServer>
#include "serverLog.h"
#include <QObject>

class sslServer : public QSslServer, public QRunnable
{
    Q_OBJECT
public:
    explicit sslServer(QObject *parent = nullptr, const QString& logFileName = "server-logs/server-log.txt");
    ~sslServer();

    void log(const ServerLog::MessageType& type, const QString& text);
public slots:
    void errorProcess(const QSslSocket* socket, const QAbstractSocket::SocketError& error);

protected:
    void run() override;
private:
    ServerLog* logger;

    void init();
    void signalProcess();
    void sslSocketReady(QSslSocket* sslSocket);
};

#endif // TCPSERVER_H
