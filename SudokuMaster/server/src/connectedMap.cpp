/**
  ******************************************************************************
  * @file           : ConnectedMap.cpp
  * @author         : hui
  * @brief          : None
  * @attention      : None
  * @date           : 2023/7/1
  ******************************************************************************
  */

#include "connectedMap.h"
#include <QThreadPool>
#include <QMutexLocker>
#include "sslServer.h"

ConnectedMap::ConnectedMap() :
        QRunnable(),
        connectedMap(new QMap<QSharedPointer<QSslSocket>, QSharedPointer<SslSocketThread>>) {
    setAutoDelete(false);
}

ConnectedMap::~ConnectedMap() {
    qDebug() << "~ConnectedMap()";
    delete connectedMap;
}

void ConnectedMap::run() {
    signalProcess();
}

void ConnectedMap::signalProcess() {
    connect(this, &ConnectedMap::insertToMap, this, &ConnectedMap::insertAndStart);
}

void ConnectedMap::insertAndStart(const QSharedPointer<QSslSocket> &sharedSslSocket,
                                  const QSharedPointer<SslSocketThread> &sharedSslSocketThread) {
    qDebug() << "ConnectedMap::insert()";

    connectedMap->insert(sharedSslSocket, sharedSslSocketThread);
    QThreadPool::globalInstance()->start(sharedSslSocketThread.get());

    //When the connection is disconnected, remove the connection from the map
    connect(sharedSslSocket.get(), &QSslSocket::disconnected, this, [&](){
        qDebug() << "delete connection in map";
        connectedMap->remove(sharedSslSocket);
    });
}
