
#ifndef LOGSERVERSINGLETON_H
#define LOGSERVERSINGLETON_H

#include "spdlog/logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <QString>
#include <memory>

class LogServerSingleton {
public:
    enum messageType {
        _TRACE,
        _DEBUG,
        _INFO,
        _WARN,
        _ERROR,
        _CRITICAL
    };
//    void log(const LogServerSingleton::messageType &type, const QString &message);

    LogServerSingleton(const LogServerSingleton::messageType &type, const QString &message);
    LogServerSingleton(const LogServerSingleton &)            = delete;
    LogServerSingleton &operator=(const LogServerSingleton &) = delete;
    ~LogServerSingleton();

    void operator()(const LogServerSingleton::messageType &type, const QString &message);

private:
    std::shared_ptr<spdlog::logger> my_logger;
};

#endif // LOGSERVERSINGLETON_H
