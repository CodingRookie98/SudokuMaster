//
// Created by hui on 2023/6/26.
//

#include "tcpService.h"
#include "LogServer.h"

static const QHostAddress hostAddress(QString("180.76.76.76"));
static const qint16 port(4567);

tcpService::tcpService(QObject* parent) : QTcpSocket(parent){
    this->connectToHost(hostAddress, port);
    connect(this, &QTcpSocket::connected, this, [](){
        LogServer(LogServer::MessageType::_INFO, QString("connect to server"));
        qDebug() << "connect success";
    });
    connect(this, &QTcpSocket::errorOccurred, this, [&](){
        LogServer(LogServer::MessageType::_ERROR, QString(this->errorString()));
    });
}

void tcpService::readFromServer() {

}

//void tcpService::dataFromServerReady(const tcpService::RequestType &type, const QByteArray &data) {
//
//}

void tcpService::writeToServer(const tcpService::RequestType &type, const QByteArray &data) {

}
