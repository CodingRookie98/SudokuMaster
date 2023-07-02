#include "globalLogger.h"
#include <QDebug>
#include "spdlog/sinks/daily_file_sink.h"

GlobalLogger::GlobalLogger(): QRunnable() {
    qDebug() << "GlobalLogger init";
    mLogger = spdlog::daily_logger_mt<spdlog::async_factory>("user", "client-logs/log.txt", 23, 59);
    mLogger->set_level(spdlog::level::level_enum::trace);
    setAutoDelete(false);
}

GlobalLogger::~GlobalLogger() {
    qDebug() << "star to destroy";
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
}

GlobalLogger &GlobalLogger::getInstance() {
    static GlobalLogger instance;
    return instance;
}

void GlobalLogger::run() {

}
