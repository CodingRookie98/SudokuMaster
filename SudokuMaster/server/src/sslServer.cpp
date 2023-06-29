
#include "sslServer.h"
#include "LogServer.h"
#include <QSslServer>
#include <QSslConfiguration>
#include <QSslCertificate>
#include <QFile>
#include <QSslKey>

sslServer::sslServer(QObject *parent) {

    init();
    signalProcess();
}

void sslServer::init() {

    QSslConfiguration sslConfig;
    QSslCertificate ca;
    QSslKey sslKey;

    QFile caFile("C:/0_WorkSpace/QT_Learning/SudokuMaster/SudokuMaster/CA/Server/server.cer");
    QFile key("C:/0_WorkSpace/QT_Learning/SudokuMaster/SudokuMaster/CA/Server/server_no_passwd.key");

    if (caFile.open(QIODevice::ReadOnly) && key.open(QIODevice::ReadOnly)) {
        ca = QSslCertificate(&caFile, QSsl::EncodingFormat::Der);
        sslKey = QSslKey(&key, QSsl::Rsa);
    } else {
        this->log(LogServer::MessageType::Error, QString("failed to open server.cer"));
    }
    caFile.close();

    if (ca.isNull()) {
        qDebug() << "ca is NULL";
    } else {
        // 设置证书
        sslConfig.setLocalCertificate(ca);
        // 不要求客户端提供证书，但仍然是加密通信
        sslConfig.setPeerVerifyMode(QSslSocket::PeerVerifyMode::VerifyNone);
        // 设置私钥，注意这里设置的私钥不能带有密码的，不然会导致握手失败
        sslConfig.setPrivateKey(sslKey);
        // 设置通信协议，默认协议是已知安全的协议
        // sslConfig.setProtocol(QSsl::SslProtocol::TlsV1_2OrLater);
    }

    this->setSslConfiguration(sslConfig);

    if (!this->listen(QHostAddress::Any, 45678)) {
        this->log(LogServer::MessageType::Error, QString("Failed to create server"));
    } else if (this->isListening()) {
        this->log(LogServer::MessageType::Info, QString("Listening on port " + QString::number(this->serverPort())));
    }
}

void sslServer::signalProcess() {
    connect(this, &QSslServer::pendingConnectionAvailable, this, [&]() {
        QTcpSocket *s = this->nextPendingConnection();
        qDebug() << QString("Connection established ") + s->peerAddress().toString() + ":"
                    + QString::number(s->peerPort());
        LogServer::getInstance().log(LogServer::MessageType::Info,
                                     QString("Connection established ") + s->peerAddress().toString() + ":"
                                     + QString::number(s->peerPort()));
    });

    connect(this, &QSslServer::newConnection, this, [&]() {
        qDebug() << "A new connection is coming";
        LogServer::getInstance().log(LogServer::MessageType::Debug, "A new connection is coming");
    });

    connect(this, &QSslServer::handshakeInterruptedOnError, this, &sslServer::errorProcess);

    connect(this, &sslServer::peerVerifyError, this, &sslServer::errorProcess);
}

void sslServer::log(const LogServer::MessageType &type, const QString &text) {
    qDebug() << text;
    LogServer::getInstance().log(type, text);
}
