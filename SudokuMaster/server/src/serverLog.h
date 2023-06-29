#ifndef LOGSERVER_H
#define LOGSERVER_H

#ifdef _WIN32
#include "windows.h"
#endif

#include <QString>
//#include "spdlog/common.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"

class ServerLog{
public:
    enum MessageType {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };
    ServerLog(const QString& logFileName);

    void log(const ServerLog::MessageType& type, const QString& message);
    ~ServerLog();

private:
    std::shared_ptr<spdlog::logger> mLogger;
};

#endif // LOGSERVERSINGLETON_H
