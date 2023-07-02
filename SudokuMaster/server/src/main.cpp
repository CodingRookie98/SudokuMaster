//
// Created by hui on 2023/6/26.
//

#include <QCoreApplication>
#include "sslServer.h"
#include "QDebug"
#include <QThreadPool>
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    sslServer *server = nullptr;
    try {
        server = new sslServer;
        QThreadPool::globalInstance()->start(server);
        server->log(ServerLog::Info, "start work");

        // 必须把监听端口放在主线程中执行，否则会
        // QObject: Cannot create children for a parent that is in a different thread.
        if (!server->listen(QHostAddress::Any, qint16(45678))) {
            server->log(ServerLog::MessageType::Error, QString("Failed to create server"));
            delete server;
            QCoreApplication::exit();
        } else if (server->isListening()) {
            server->log(ServerLog::MessageType::Info,
                        QString("Listening on port " + QString::number(server->serverPort())));
        }
    } catch (const std::bad_alloc &e) {
        std::cout << "Failed to allocate memory: " << e.what() << std::endl;
        QCoreApplication::exit();
    }

    // Wait for all threads in the thread pool to complete their work before exiting
    QThreadPool::globalInstance()->waitForDone(-1);
    return QCoreApplication::exec();
}
