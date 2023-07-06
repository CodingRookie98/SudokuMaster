/**
  ******************************************************************************
  * @file           : sslSocketMessage.h
  * @author         : hui
  * @brief          : None
  * @attention      : None
  * @date           : 2023/7/2
  ******************************************************************************
  */

#ifndef SUDOKUMASTER_SSLSOCKETMESSAGE_H
#define SUDOKUMASTER_SSLSOCKETMESSAGE_H

#include <QMap>
#include <QString>


namespace sslSocketMessage {

    /* ============= Request type ============= */
    static const QString keyRequestType("RequestType");
    enum RequestType {
        SignUp,
        LoginIn
    };

    static QMap<RequestType, QString> RequestTypeQStr{
            {RequestType::SignUp,  "SignUp"},
            {RequestType::LoginIn, "LoginIn"}
    };

    static QMap<QString, RequestType> QStrRequestType{
            {"SignUp",  RequestType::SignUp},
            {"LoginIn", RequestType::LoginIn}
    };

    /* ============= Request type ============= */


    /* ============= Response type ============= */
    static const QString keyResponseType("ResponseType");
    enum ResponseType {
        UsernameAlreadyExists,
        UserNotExist,
        WrongPassword,
        SignUpSuccess,
        LoginInSuccess,
    };

    static QMap<ResponseType, QString> ResponseTypeQStr{
            {ResponseType::UsernameAlreadyExists, "UsernameAlreadyExists"},
            {ResponseType::UserNotExist,          "UserNotExist"},
            {ResponseType::WrongPassword,         "WrongPassword"},
            {ResponseType::SignUpSuccess,         "SignUpSuccess"},
            {ResponseType::LoginInSuccess,        "LoginInSuccess"},
    };

    static QMap<QString, ResponseType> QStrResponseType{
            {"UsernameAlreadyExists", ResponseType::UsernameAlreadyExists},
            {"UserNotExist",          ResponseType::UserNotExist,},
            {"WrongPassword",         ResponseType::WrongPassword,},
            {"SignUpSuccess",         ResponseType::SignUpSuccess,},
            {"LoginInSuccess",        ResponseType::LoginInSuccess,},
    };
    /* ============= Response type ============= */


    /* ============= user info ============= */
    enum UserInfoType {
        Username,
        Password,
    };

    static QMap<UserInfoType, QString> UserInfoTypeQStr{
            {UserInfoType::Username, "Username"},
            {UserInfoType::Password, "Password"},
    };

    static QMap<QString, UserInfoType> QStrUserInfoType{
            {"Username", UserInfoType::Username},
            {"Password", UserInfoType::Password},
    };
    /* ============= user info ============= */

}

#endif //SUDOKUMASTER_SSLSOCKETMESSAGE_H
