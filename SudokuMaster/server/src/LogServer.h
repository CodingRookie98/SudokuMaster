#ifndef LOGSERVER_H
#define LOGSERVER_H

#include <QString>
#include <memory>
#include <QSharedPointer>
#include "windows.h"
#include "spdlog/common.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"

class LogServer{
public:
    enum MessageType {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };
    static LogServer& getInstance();
    LogServer(const LogServer &) = delete;
    LogServer &operator=(const LogServer &) = delete;

    void log(const LogServer::MessageType& type, const QString& message);
    ~LogServer();
private:

    LogServer();
    std::shared_ptr<spdlog::logger> mLogger;
};

#endif // LOGSERVERSINGLETON_H
