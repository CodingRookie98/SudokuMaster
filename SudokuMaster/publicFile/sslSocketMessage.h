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
    QString keyMessageOwner("MessageOwner");

    enum MessageOwner {
        LoginWnd,
    };

    static QMap<MessageOwner, QString> MessageOwnerQStr{
            {MessageOwner::LoginWnd, "LoginWnd"},
    };

    static QMap<QString, MessageOwner> QStrMessageOwner;

    enum UserInfoType {
        Username,
        Password,
    };

    static QMap<UserInfoType, QString> UserInfoTypeQStr {
            {UserInfoType::Username, "Username"},
            {Password, "Password"},
    };

    static QMap<QString, UserInfoType> QStrUserInfoType;

    enum ClientMessageType {
        SignUp,
        LoginIn
    };

    static QMap<ClientMessageType, QString> ClientMessageTypeQStr{
            {ClientMessageType::SignUp,  "SignUp"},
            {ClientMessageType::LoginIn, "LoginIn"}
    };

    static QMap<QString, ClientMessageType> QStrClientMessageType;

    enum ServerMessageType {
        UsernameAlreadyExists,
        UserNotExist,
        WrongPassword,
        SignUpSuccess,
    };

    static QMap<ServerMessageType, QString> ServerMessageTypeQStr{
            {ServerMessageType::UsernameAlreadyExists, "UsernameAlreadyExists"},
            {ServerMessageType::UserNotExist,          "UserNotExist"},
            {ServerMessageType::WrongPassword,         "WrongPassword"},
            {ServerMessageType::SignUpSuccess,         "SignUpSuccess"},
    };

    static QMap<QString, ServerMessageType> QStrServerMessageType;


    void init();
}

#endif //SUDOKUMASTER_SSLSOCKETMESSAGE_H
