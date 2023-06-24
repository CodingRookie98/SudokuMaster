
#include "DatabaseSingleton.h"
#include <QDebug>

DatabaseSingleton &DatabaseSingleton::getInstance() {
    static DatabaseSingleton instance; // 在静态函数内部创建唯一的实例
    return instance;
}

DatabaseSingleton::DatabaseSingleton() :
    QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL")) {
    // 连接数据库的逻辑
    setDatabaseName("sudoku_master_test");
    setUserName("client_sudokumaster");
    setPassword("SudokuMaster");
    setHostName("localhost");
    setPort(3306);

    if (open()) {
        qDebug() << "Database connected!";
    } else {
        qDebug() << "Failed to connect to database!";
    }
}
DatabaseSingleton::~DatabaseSingleton() {
    // 清理代码
    close();
    QSqlDatabase::removeDatabase(defaultConnection);
}
