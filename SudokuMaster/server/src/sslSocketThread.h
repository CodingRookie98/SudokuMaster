/**
  ******************************************************************************
  * @file           : sslSocketThread.h
  * @author         : hui
  * @brief          : None
  * @attention      : None
  * @date           : 2023/6/30
  ******************************************************************************
  */

#ifndef SUDOKUMASTER_SSLSOCKETTHREAD_H
#define SUDOKUMASTER_SSLSOCKETTHREAD_H

#include <QSslSocket>
#include <QRunnable>

class SslSocketThread : public QObject, public QRunnable {
    Q_OBJECT
public:
    explicit SslSocketThread(const QSharedPointer<QSslSocket>& sslSocket);
    ~SslSocketThread() override;
protected:
    void run() override;
private:
    QSharedPointer<QSslSocket> mSslSocket;

    void init();
    void signalProcess();
};


#endif //SUDOKUMASTER_SSLSOCKETTHREAD_H
