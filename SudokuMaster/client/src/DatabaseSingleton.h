
#ifndef DATABASESINGLETON_H
#define DATABASESINGLETON_H

#include <QSqlDatabase>

class DatabaseSingleton : public QSqlDatabase {
public:
    static DatabaseSingleton& getInstance();
    // 禁用拷贝构造函数和赋值运算符
    DatabaseSingleton(const DatabaseSingleton&) = delete;
    DatabaseSingleton& operator=(const DatabaseSingleton&) = delete;


private:
    // 私有构造函数，防止外部实例化
    DatabaseSingleton();
    // 私有析构函数，防止外部删除实例
    ~DatabaseSingleton();
};

#endif // DATABASESINGLETON_H
