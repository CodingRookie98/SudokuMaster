//
// Created by hui on 2023/6/26.
//

#include <QCoreApplication>
#include "sslServer.h"
#include "QDebug"
#include <QThreadPool>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    auto *server = new sslServer;
    while (server == nullptr) {
        server = new sslServer(&a);
    }

    QThreadPool::globalInstance()->start(server);
    server->log(ServerLog::Info, "start work");

    // 必须把监听端口放在主线程中执行，否则会
    // QObject: Cannot create children for a parent that is in a different thread.
    if (!server->listen(QHostAddress::Any, qint16(45678))) {
        server->log(ServerLog::MessageType::Error, QString("Failed to create server"));
        delete server;
        a.exit();
    }else if (server->isListening()) {
        server->log(ServerLog::MessageType::Info,
                       QString("Listening on port " + QString::number(server->serverPort())));
    }

    return a.exec();
}
