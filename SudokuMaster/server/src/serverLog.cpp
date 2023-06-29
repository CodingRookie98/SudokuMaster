#include "serverLog.h"
#include <QDebug>
#include <QDateTime>

ServerLog::ServerLog(const QString &logFileName) {
    qDebug() << "LogServer init";
    mLogger = spdlog::basic_logger_mt<spdlog::async_factory>("server", logFileName.toStdString());
    mLogger->set_level(spdlog::level::level_enum::trace);
}

void ServerLog::log(const ServerLog::MessageType &type, const QString &message) {
    qDebug() << "write " + message << " to log";
    switch (type) {
        case MessageType::Trace:
            mLogger->trace(message.toStdString());
            break;
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
            break;
        default:
            break;
    }
    // 解决内容不写入文件的问题
    mLogger->flush();
}

ServerLog::~ServerLog() {
    qDebug() << "star to destroy ServerLog";
    mLogger.reset();
}
