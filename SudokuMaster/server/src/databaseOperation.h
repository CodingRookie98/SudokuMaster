/**
  ******************************************************************************
  * @file           : databaseOperation.h
  * @author         : hui
  * @brief          : None
  * @attention      : None
  * @date           : 2023/7/2
  ******************************************************************************
  */

#ifndef SUDOKUMASTER_DATABASEOPERATION_H
#define SUDOKUMASTER_DATABASEOPERATION_H
#include <QString>
#include <QMap>

enum DatabaseOperationType {
    AddUser,
    SelectUserName,
    IsCorrectPassword,
};

static QMap<DatabaseOperationType, QString> DatabaseOperationQStr {
        {DatabaseOperationType::AddUser, "SELECT addUser('%1', '%2')"}, // %1: userName, %2: password
        {DatabaseOperationType::SelectUserName, "SELECT selectUserName('%1')"}, // %1: userName
        {DatabaseOperationType::IsCorrectPassword, "SELECT isCorrectPassword('%1', '%2')"}, // %1: userName, %2: password
};
#endif //SUDOKUMASTER_DATABASEOPERATION_H
