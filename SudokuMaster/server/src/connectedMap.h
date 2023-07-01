/**
  ******************************************************************************
  * @file           : connectionMap.h
  * @author         : hui
  * @brief          : None
  * @attention      : None
  * @date           : 2023/7/1
  ******************************************************************************
  */

#ifndef SUDOKUMASTER_CONNECTEDMAP_H
#define SUDOKUMASTER_CONNECTEDMAP_H

#include <QRunnable>
#include <QSslSocket>
#include <QMutex>
#include "sslSocketThread.h"

class ConnectedMap : public QObject, public QRunnable {
    Q_OBJECT

public:
    ConnectedMap();

    ~ConnectedMap();

signals:

    void insertToMap(QSharedPointer<QSslSocket> &, QSharedPointer<SslSocketThread> &);

    void connectionDisconnected(QSharedPointer<QSslSocket> &sharedSslSocket);

protected:
    void run() override;

private:
    QMutex mutex;
    QMap<QSharedPointer<QSslSocket>, QSharedPointer<SslSocketThread>> *connectedMap;

    void signalProcess();

private slots:

    void insertAndStart(QSharedPointer<QSslSocket> &sslSocket, QSharedPointer<SslSocketThread> &sslSocketThread);
};


#endif //SUDOKUMASTER_CONNECTEDMAP_H
