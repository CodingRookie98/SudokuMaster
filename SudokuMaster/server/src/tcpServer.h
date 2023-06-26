
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>



class tcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit tcpServer(QObject *parent = nullptr);
};

#endif // TCPSERVER_H
