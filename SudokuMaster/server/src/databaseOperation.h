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

namespace databaseOperation {
    enum DatabaseOperationType {
        add_user,
        select_user_name,
        is_correct_password,
    };

    static QMap<DatabaseOperationType, QString> DatabaseOperationQStr{
            {DatabaseOperationType::add_user,            "SELECT add_user('%1', '%2')"}, // %1: userName, %2: password
            {DatabaseOperationType::select_user_name,    "SELECT select_user_name('%1')"}, // %1: userName
            {DatabaseOperationType::is_correct_password, "SELECT is_correct_password('%1', '%2')"}, // %1: userName, %2: password
    };
}
#endif //SUDOKUMASTER_DATABASEOPERATION_H
