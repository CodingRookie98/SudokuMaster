#include "LogServer.h"
#include <QDebug>
#include <QDateTime>

QFile LogServer::logFile("log.txt");

LogServer::LogServer(const MessageType &type, const QString &message) {
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        (*this)(type, message);
    } else {

    }

}

LogServer::~LogServer() {
    logFile.close();
}

void LogServer::operator()(const MessageType &type, const QString &message) {
    static const std::unordered_map<MessageType, QString>
            levelMap = {
            {MessageType::_TRACE,    "[trace]"},
            {MessageType::_DEBUG,    "[debug]"},
            {MessageType::_INFO,     "[info]"},
            {MessageType::_WARN,     "[warn]"},
            {MessageType::_ERROR,    "[error]"},
            {MessageType::_CRITICAL, "[critical"}};

    auto levelIter = levelMap.find(type);
    if (levelIter != levelMap.end()) {
        QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QTextStream textStream(&logFile);
        textStream << "[" << currentDateTime << "]" << levelIter->second << message;
    }
}
