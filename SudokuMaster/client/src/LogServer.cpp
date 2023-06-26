#include "LogServer.h"
#include "spdlog/common.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <winsvc.h>
#include <windows.h>
#include <QDebug>

LogServer::LogServer(const MessageType &type, const QString &message) :
    my_logger(spdlog::basic_logger_mt("file_logger", "log.txt", true)) {
    my_logger->set_level(spdlog::level::trace);
    (*this)(type, message);
}

LogServer::~LogServer() {
    my_logger.reset();
}

void LogServer::operator()(const MessageType &type, const QString &message) {
    static const std::unordered_map<MessageType, spdlog::level::level_enum>
        levelMap = {
            {MessageType::_TRACE, spdlog::level::trace},
            {MessageType::_DEBUG, spdlog::level::debug},
            {MessageType::_INFO, spdlog::level::info},
            {MessageType::_WARN, spdlog::level::warn},
            {MessageType::_ERROR, spdlog::level::err},
            {MessageType::_CRITICAL, spdlog::level::critical}};

    auto levelIter = levelMap.find(type);
    if (levelIter != levelMap.end()) {
        my_logger->log(levelIter->second, message.toStdString());
    }
}
