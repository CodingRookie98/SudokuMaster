#include "LogServer.h"
#include <QDebug>
#include <QDateTime>

void LogServer::log(const LogServer::MessageType &type, const QString &message) {
    switch (type) {
        case MessageType::Trace:
            mLogger->trace(message.toStdString());
        case MessageType::Debug:
            mLogger->debug(message.toStdString());
            break;
        case MessageType::Info:
            mLogger->info(message.toStdString());
            break;
        case MessageType::Warn:
            mLogger->warn(message.toStdString());
            break;
        case MessageType::Error:
            mLogger->error(message.toStdString());
            break;
        case MessageType::Critical:
            mLogger->critical(message.toStdString());
        default:
            break;
    }
}


LogServer::LogServer() {
    QString formattedTime = QDateTime::currentDateTime().toString("yy-MM-dd-HH");
    qDebug() << "LogServer init";
    mLogger = spdlog::basic_logger_mt<spdlog::async_factory>("user", formattedTime.toStdString() + "-logC.txt");
    mLogger->set_level(spdlog::level::level_enum::trace);
}

LogServer::~LogServer() {
    qDebug() << "star to destroy";
    mLogger.reset();
}

LogServer &LogServer::getInstance() {
    static LogServer instance;
    return instance;
}
