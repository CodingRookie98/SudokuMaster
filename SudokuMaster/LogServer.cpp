#include "LogServer.h"
#include "spdlog/common.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <winsvc.h>
#include <QDebug>

LogServer::LogServer(const messageType &type, const QString &message) {
    my_logger = spdlog::basic_logger_mt("file_logger", "log.txt", true);
    my_logger->set_level(spdlog::level::trace);
    (*this)(type, message);
}

LogServer::~LogServer() {
    my_logger.reset();
}

void LogServer::operator()(const messageType &type, const QString &message) {
    switch (type) {
    case _TRACE:
        my_logger->trace(message.toStdString());
        break;
    case _DEBUG:
        my_logger->debug(message.toStdString());
        break;
    case _CRITICAL:
        my_logger->critical(message.toStdString());
        break;
    case _ERROR:
        my_logger->error(message.toStdString());
        break;
    case _INFO:
        my_logger->info(message.toStdString());
        break;
    case _WARN:
        my_logger->warn(message.toStdString());
        break;
    default:
        break;
    }
}
