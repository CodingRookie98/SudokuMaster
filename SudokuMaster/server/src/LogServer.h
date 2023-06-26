#ifndef LOGSERVER_H
#define LOGSERVER_H

#include <QString>
#include <memory>
#include <QFile>

class LogServer {
public:
    enum MessageType {
        _TRACE,
        _DEBUG,
        _INFO,
        _WARN,
        _ERROR,
        _CRITICAL
    };

    LogServer(const LogServer::MessageType &type, const QString &message);
    LogServer(const LogServer &)            = delete;
    LogServer &operator=(const LogServer &) = delete;
    ~LogServer();

    void operator()(const LogServer::MessageType &type, const QString &message);

private:
    static QFile logFile;
};

#endif // LOGSERVERSINGLETON_H
