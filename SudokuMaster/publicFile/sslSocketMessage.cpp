/**
  ******************************************************************************
  * @file           : sslSocketMessage.cpp
  * @author         : hui
  * @brief          : None
  * @attention      : None
  * @date           : 2023/7/2
  ******************************************************************************
  */

#include "sslSocketMessage.h"

void sslSocketMessage::init() {

    for (auto itr = MessageOwnerQStr.constBegin(); itr != MessageOwnerQStr.constEnd(); ++itr) {
        auto key = itr.key();
        const auto& val = itr.value();
        QStrMessageOwner.insert(val, key);
    }

    for (auto itr = ClientMessageTypeQStr.constBegin(); itr != ClientMessageTypeQStr.constEnd(); ++itr) {
        auto key = itr.key();
        const auto& val = itr.value();
        QStrClientMessageType.insert(val, key);
    }

    for (auto itr = ServerMessageTypeQStr.constBegin(); itr != ServerMessageTypeQStr.constEnd(); ++itr) {
        auto key = itr.key();
        const auto& val = itr.value();
        QStrServerMessageType.insert(val, key);
    }
    
    for (auto itr = UserInfoTypeQStr.constBegin(); itr != UserInfoTypeQStr.constEnd(); ++itr) {
        auto key = itr.key();
        const auto& val = itr.value();
        QStrUserInfoType.insert(val, key);
    }
}