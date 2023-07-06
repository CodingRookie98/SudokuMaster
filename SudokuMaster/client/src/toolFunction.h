/**
  ******************************************************************************
  * @file           : toolFunction.h
  * @author         : hui
  * @brief          : None
  * @attention      : None
  * @date           : 2023/7/6
  ******************************************************************************
  */

#ifndef SUDOKUMASTER_TOOLFUNCTION_H
#define SUDOKUMASTER_TOOLFUNCTION_H

#include <QByteArray>
#include <openssl/aes.h>


namespace toolFunction {

    QByteArray encrypt(const QByteArray &key, const QByteArray &iv, const QByteArray &data);

    QByteArray decrypt(const QByteArray &key, const QByteArray &iv, const QByteArray &encryptedData);
} // toolFunction

#endif //SUDOKUMASTER_TOOLFUNCTION_H
