
#include "sslServer.h"
#include <QSslServer>
#include <QSslConfiguration>
#include <QSslCertificate>
#include <QFile>
#include <QSslKey>
#include <QThreadPool>
#include "sslSocketThread.h"
#include <u

sslServer::sslServer(QObject *parent, const QString &logFileName) :
        QSslServer(parent),
        logger(new ServerLog(logFileName)),
        QRunnable() {

    setAutoDelete(false);
}

void sslServer::init() {

    QSslConfiguration sslConfig;
    QSslCertificate ca;
    QSslKey sslKey;

    QFile caFile(":/CA/CA/server.crt");
    QFile key(":/CA/CA/server_no_passwd.key");

    if (caFile.open(QIODevice::ReadOnly) && key.open(QIODevice::ReadOnly)) {
        ca = QSslCertificate(&caFile);
        sslKey = QSslKey(&key, QSsl::Rsa);
    } else {
        this->log(ServerLog::MessageType::Error, QString("failed to open server.cer"));
    }
    caFile.close();

    if (ca.isNull()) {
        this->log(ServerLog::MessageType::Critical, QString("ca is NULL"));
    } else {
        // 设置证书
        sslConfig.setLocalCertificate(ca);
        // VerifyNone不要求客户端提供证书，但仍然是加密通信
        sslConfig.setPeerVerifyMode(QSslSocket::PeerVerifyMode::VerifyNone);
        // 设置私钥，注意这里设置的私钥不能带有密码的，不然会导致握手失败
        sslConfig.setPrivateKey(sslKey);
        // 设置通信协议，默认协议是已知安全的协议
        // sslConfig.setProtocol(QSsl::SslProtocol::TlsV1_2OrLater);
    }

    this->setSslConfiguration(sslConfig);

}

void sslServer::signalProcess() {

    connect(this, &QSslServer::pendingConnectionAvailable, this, [&]() {
        auto *sslSocket = dynamic_cast<QSslSocket *>(this->nextPendingConnection());
        this->log(ServerLog::MessageType::Info,
                  sslSocket->peerAddress().toString() + ":"
                  + QString::number(sslSocket->peerPort()) + QString(" Connected"));
        sslSocketReady(sslSocket);
    });

    connect(this, &QSslServer::newConnection, this, [&]() {
        this->log(ServerLog::MessageType::Info, "A new connection is coming");
    });

    // 处理在在服务器监听期间发生握手时产生的错误
    connect(this, &QSslServer::errorOccurred, this, &sslServer::errorProcess);
}

void sslServer::log(const ServerLog::MessageType &type, const QString &text) {
//    qDebug() << text;
    QThreadPool::globalInstance()->start(logger);
    logger->log(type, text);
}

void sslServer::errorProcess(const QSslSocket *socket, const QAbstractSocket::SocketError &error) {
    this->log(ServerLog::MessageType::Error,
              QString(socket->peerAddress().toString() + ":"
                      + QString::number(socket->peerPort()) + " " + socket->errorString()));
}

sslServer::~sslServer() {
    delete logger;
}

void sslServer::run() {
    init();
    signalProcess();
}

void sslServer::sslSocketReady(QSslSocket *sslSocket) {
    // Todo : 
    static QSharedPointer<QSslSocket> sharedSslSocket(sslSocket);
    static QSharedPointer<SslSocketThread> mSslSocketThread((new SslSocketThread(sharedSslSocket)));

    QThreadPool::globalInstance()->start(mSslSocketThread.get());
    connect(sharedSslSocket.get(), &QSslSocket::disconnected, this, [&]() {
        this->log(ServerLog::MessageType::Info, QString(sharedSslSocket->peerAddress().toString() + ":"
                                                        + QString::number(sharedSslSocket->peerPort()) + " "
                                                        + "Disconnected"));
        mSslSocketThread.reset();
        sharedSslSocket.reset();
        qDebug() << sharedSslSocket.isNull();
    });
}
