#include "serverLog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/hourly_file_sink.h"
#include "spdlog/async.h"

ServerLog::ServerLog(const QString &logFileName) :  QRunnable() {
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    sinks.push_back(std::make_shared<spdlog::sinks::hourly_file_sink_mt>(logFileName.toStdString()));
    mLogger = std::make_shared<spdlog::logger>("server", sinks.begin(), sinks.end());
    mLogger->set_level(spdlog::level::level_enum::trace);
    setAutoDelete(false);
}

void ServerLog::log(const ServerLog::MessageType &type, const QString &message) const {
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
    mLogger.reset();
}

void ServerLog::run() {
    
}
