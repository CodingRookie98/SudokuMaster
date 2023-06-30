/**
  ******************************************************************************
  * @file           : sslSocketThread.cpp
  * @author         : hui
  * @brief          : None
  * @attention      : None
  * @date           : 2023/6/30
  ******************************************************************************
  */

#include "sslSocketThread.h"

SslSocketThread::SslSocketThread(const QSharedPointer<QSslSocket>& sslSocket) : QRunnable(), mSslSocket(sslSocket) {
    setAutoDelete(false);
}

SslSocketThread::~SslSocketThread() {
    mSslSocket.reset();
}

void SslSocketThread::run() {
    int();
    signalProcess();
}

void SslSocketThread::init() {

}

void SslSocketThread::signalProcess() {
//    connect(mSslSocket.get(), &QSslSocket::stateChanged, this, [](const QAbstractSocket::SocketState& state){
//        qDebug() << "@@" << qt_error_string(state);
//    });
}
