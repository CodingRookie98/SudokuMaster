#ifndef GLOBALLOGGER_H
#define GLOBALLOGGER_H
#include <QString>
#include <QSharedPointer>
#include <QRunnable>
#include "spdlog/spdlog.h"

class GlobalLogger : public QRunnable{
public:
    enum MessageType {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };
    static GlobalLogger& getInstance();
    GlobalLogger(const GlobalLogger &) = delete;
    GlobalLogger &operator=(const GlobalLogger &) = delete;

    void log(const GlobalLogger::MessageType& type, const QString& message);
    ~GlobalLogger() override;
protected:
    void run() override;
private:

    GlobalLogger();
    std::shared_ptr<spdlog::logger> mLogger;
};

#endif // GLOBALLOGGER_H
