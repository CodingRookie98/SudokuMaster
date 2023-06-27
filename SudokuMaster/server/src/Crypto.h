//
// Created by hui on 2023/6/27.
//

#ifndef SUDOKUMASTER_CRYPTO_H
#define SUDOKUMASTER_CRYPTO_H

#include <QByteArray>
#include <QDebug>
#include <QCryptographicHash>
#include <QDataStream>
#include <QIODevice>

#include <openssl/aes.h>

QByteArray encryptData(const QByteArray& data, const QByteArray& key);

QByteArray decryptData(const QByteArray& encryptedData, const QByteArray& key)

#endif //SUDOKUMASTER_CRYPTO_H
