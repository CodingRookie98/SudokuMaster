#ifndef LOGSERVER_H
#define LOGSERVER_H

#include <QString>
#include <QRunnable>
#include <QThread>
#include "spdlog/sinks/basic_file_sink.h"

class ServerLog : public QRunnable {
public:
    enum MessageType {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };
    explicit ServerLog(const QString& logFileName);
    ~ServerLog() override;

    void log(const ServerLog::MessageType& type = Debug, const QString& message = "") const;

protected:
    void run() override;

private:
    std::shared_ptr<spdlog::logger> mLogger;
};

#endif // LOGSERVER_H
