#include <QDebug>
#include "globalLogger.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/async.h"

GlobalLogger::GlobalLogger(): QRunnable() {
    qDebug() << "GlobalLogger construct";
    mLogger = spdlog::daily_logger_mt<spdlog::async_factory>("user", "client-logs/log.txt", 23, 59);
    mLogger->set_level(spdlog::level::level_enum::trace);
    setAutoDelete(false);
}

GlobalLogger::~GlobalLogger() {
    qDebug() << "destroy the GlobalLogger";
    mLogger.reset();
}


void GlobalLogger::log(const GlobalLogger::MessageType &type, const QString &message) {
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
    mLogger->flush();
}

GlobalLogger &GlobalLogger::getInstance() {
    static GlobalLogger instance;
    return instance;
}

void GlobalLogger::run() {

}
