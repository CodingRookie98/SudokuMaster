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

void ConnectedMap::insertAndStart(QSharedPointer<QSslSocket> &sslSocket, QSharedPointer<SslSocketThread> &sslSocketThread) {
    qDebug() << "ConnectedMap::insert()";

    auto iter = connectedMap->insert(sslSocket, sslSocketThread);
    QThreadPool::globalInstance()->start(iter.value().get());

    connect(this, &ConnectedMap::connectionDisconnected,
            this, [&](QSharedPointer<QSslSocket>& sharedSslSocket){
        qDebug() << "delete in connectedMap";
        connectedMap->remove(sharedSslSocket);
    });
}
