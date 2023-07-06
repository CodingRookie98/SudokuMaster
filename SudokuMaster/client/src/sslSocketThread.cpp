//
// Created by hui on 2023/6/26.
//

#include <QFile>
#include <QSslConfiguration>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include "sslSocketMessage.h"
#include "sslSocketThread.h"
#include "GlobalLogger.h"
#include "QSslSocket"


SslSocketThread::SslSocketThread(QObject *parent) : QSslSocket(parent) {
    setAutoDelete(false);
}

void SslSocketThread::init() {
    QSslConfiguration sslConfig;
    QSslCertificate caServer;

    QFile caServerFile(":/CA/CA/server.crt");
    if (caServerFile.open(QIODevice::ReadOnly)) {
        caServer = QSslCertificate(&caServerFile);
    } else {
        qDebug() << "failed to open server.crt";
    }
    caServerFile.close();

    if (caServer.isNull()) {
        qDebug() << "ca is NULL";
    } else {
        // Add server certificate to trust
        sslConfig.addCaCertificate(caServer);
        sslConfig.setPeerVerifyMode(QSslSocket::PeerVerifyMode::QueryPeer);
    }

    this->setSslConfiguration(sslConfig);
}

void SslSocketThread::signalProcess() {

    connect(this, &QSslSocket::encrypted, this, [&]() {
        log(GlobalLogger::MessageType::Info,
            QString("Establish a connection with the server(%1:%2)").arg(this->peerAddress().toString(),
                                                                           QString::number(this->peerPort())));
    });

    connect(this, &QSslSocket::sslErrors, this, [&](const QList<QSslError> &errors) {
                foreach(auto error, errors) {
                log(GlobalLogger::MessageType::Error, error.errorString());
            }
    });

    connect(this, &QSslSocket::stateChanged, this, [](const QAbstractSocket::SocketState &state) {
        qDebug() << state;
    });

    connect(this, &QSslSocket::disconnected, this, [&]() {
        log(GlobalLogger::MessageType::Info,
            QString("Disconnect from server(%1:%2)").arg(this->peerAddress().toString(),
                                                         QString::number(this->peerPort())));
    });

    connect(this, &QSslSocket::readyRead, this, &SslSocketThread::distributeData);

    connect(this, &SslSocketThread::readyWrite, this, &SslSocketThread::writeToServer);
}

void SslSocketThread::log(const GlobalLogger::MessageType &type, const QString &text) {
    qDebug() << text;
    GlobalLogger::getInstance().log(type, text);
}

void SslSocketThread::run() {
    init();
    signalProcess();
}

SslSocketThread::~SslSocketThread() = default;

SslSocketThread &SslSocketThread::getInstance() {
    static SslSocketThread instance;
    return instance;
}

void SslSocketThread::distributeData() {
    QByteArray byteArray = this->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
    if (!jsonDoc.isNull()) {
        if (jsonDoc.isObject() && !jsonDoc.object().isEmpty()) {
            QJsonObject jsonObject = jsonDoc.object();
            sslSocketMessage::RequestType requestType = sslSocketMessage::QStrRequestType[jsonObject.value(
                    sslSocketMessage::keyRequestType).toString()];
            switch (requestType) {
                /*
                 * 在 Qt 中，不能在 switch 语句内直接发送信号，因为 switch 语句会导致跳过 Qt 的事件循环，从而导致信号无法被正确处理。
                 * 要解决这个问题，可以使用延迟信号的机制来发送信号，以确保在事件循环中正确处理。
                 * 使用 QTimer 来延迟发送信号通过将信号的发送安排到下一个事件循环迭代中，确保信号能够在正确的上下文中被接收和处理。
                 */
                case sslSocketMessage::RequestType::SignUp :
                    // lambda 捕获列表不能为引用，因为跨线程引用的行为不被定义，会导致资源被回收时引用内容为空
                    QTimer::singleShot(0, this, [=]() {
                        emit readyReadForLoginWnd(jsonObject);
                    });
                    break;
                case sslSocketMessage::RequestType::LoginIn :
                    QTimer::singleShot(0, this, [=]() {
                        emit readyReadForLoginWnd(jsonObject);
                    });
                    break;
                default:
                    break;
            }
        } else {
            if (!jsonDoc.isObject()) {
                log(GlobalLogger::MessageType::Error, "The SslSocket received Json Document is not a Object");
            } else if (jsonDoc.object().isEmpty()) {
                log(GlobalLogger::MessageType::Error,
                    "The SslSocket received Json Document is a Object but it's empty");
            }
        }
    } else {
        log(GlobalLogger::MessageType::Error, "The SslSocket received Json Document is null");
    }
}

void SslSocketThread::writeToServer(const QJsonDocument &jsonDocument) {
    qDebug() << "SslSocketThread::writeToServer()";
    if (!jsonDocument.isNull()) {
        QByteArray byteArray = jsonDocument.toJson();
        if (!this->write(byteArray)) {
            log(GlobalLogger::MessageType::Error, "Failed to send data to server");
        }
    } else {
        log(GlobalLogger::MessageType::Error, "The jsonDocument sent to the server is null");
    }
}
