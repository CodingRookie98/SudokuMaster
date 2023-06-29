//
// Created by hui on 2023/6/26.
//

#include "sslSocket.h"
#include "LogServer.h"
#include "QSslSocket"
#include <QFile>
#include <QSslConfiguration>

const QString hostAddress("127.0.0.1");
const qint16 port(qint16(45678));

sslSocket::sslSocket(QObject *parent) : QSslSocket(parent) {
    init();
    signalProcess();
}

void sslSocket::init() {

    QSslConfiguration sslConfig;
    QSslCertificate caServer;

    QFile caServerFile("C:/0_WorkSpace/QT_Learning/SudokuMaster/SudokuMaster/CA/Server/server.cer");
    if (caServerFile.open(QIODevice::ReadOnly)) {
        caServer = QSslCertificate(&caServerFile, QSsl::EncodingFormat::Der);
    } else {
        qDebug() << "failed to open server.crt";
    }
    caServerFile.close();

    if (caServer.isNull()) {
        qDebug() << "ca is NULL";
    } else {
        // 将服务端证书加入信任
        sslConfig.addCaCertificate(caServer);
        sslConfig.setPeerVerifyMode(QSslSocket::PeerVerifyMode::QueryPeer);
    }

    this->setSslConfiguration(sslConfig);
}

void sslSocket::signalProcess() {

    this->connectToHostEncrypted(hostAddress, port);
    connect(this, &QSslSocket::encrypted, this, [&]() {
        sslSocket::log(LogServer::MessageType::Info,
                  QString("Successfully connected : ") + this->peerAddress().toString() + ":" + QString::number(this->peerPort()));

    });

    connect(this, &QSslSocket::sslErrors, this, [=](const QList<QSslError> &errors) {
                foreach(auto error, errors) {
                sslSocket::log(LogServer::MessageType::Error, error.errorString());
            }
    });
}

void sslSocket::log(const LogServer::MessageType& type, const QString& text) {
    qDebug() << text;
    LogServer::getInstance().log(type, text);
}
