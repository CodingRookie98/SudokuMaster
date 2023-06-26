
#include "tcpServer.h"
#include "LogServer.h"
#include "QTcpSocket"

tcpServer::tcpServer(QObject *parent)
    : QTcpServer{parent}
{
    if(this->listen(QHostAddress::Any, 4567) == false){
        LogServer(LogServer::MessageType::_ERROR, QString("Failed to create server"));
        return;
    }
    qDebug() << "is listening";
    connect(this, &QTcpServer::newConnection, this, [](){
        qDebug() << "A new connection is comming";
    });
    connect(this, &QTcpServer::pendingConnectionAvailable, this, [&](){
        QTcpSocket* s = this->nextPendingConnection();
       qDebug() << s->peerAddress().toString() + ":" << s->peerPort();
    });
}

