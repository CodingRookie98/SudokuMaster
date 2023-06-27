//
// Created by hui on 2023/6/27.
//

#include "Crypto.h"
#include <QOpenss

QByteArray encryptData(const QByteArray& data, const QByteArray& key)
{
    // 将密钥进行散列以生成一个32字节的密钥
    QByteArray hashedKey = QCryptographicHash::hash(key, QCryptographicHash::Sha256);

    // 使用AES-256算法加密数据
    const int keySize = 256;
    const int blockSize = 128;

    // 创建加密上下文
    AES_KEY aesKey;
    AES_set_encrypt_key((const unsigned char*)hashedKey.constData(), keySize, &aesKey);

    // 计算填充后的数据长度
    int paddedLength = ((data.length() / blockSize) + 1) * blockSize;

    // 创建填充后的数据缓冲区
    QByteArray paddedData(paddedLength, 0);

    // 将原始数据复制到填充后的缓冲区
    memcpy(paddedData.data(), data.constData(), data.length());

    // 加密数据
    QByteArray encryptedData(paddedLength, 0);
    for (int i = 0; i < paddedLength; i += blockSize)
    {
        AES_encrypt((const unsigned char*)(paddedData.constData() + i),
                    (unsigned char*)(encryptedData.data() + i),
                    &aesKey);
    }

    return encryptedData;
}

QByteArray decryptData(const QByteArray& encryptedData, const QByteArray& key)
{
    // 将密钥进行散列以生成一个32字节的密钥
    QByteArray hashedKey = QCryptographicHash::hash(key, QCryptographicHash::Sha256);

    // 使用AES-256算法解密数据
    const int keySize = 256;
    const int blockSize = 128;

    // 创建解密上下文
    AES_KEY aesKey;
    AES_set_decrypt_key((const unsigned char*)hashedKey.constData(), keySize, &aesKey);

    // 创建解密后的数据缓冲区
    QByteArray decryptedData(encryptedData.length(), 0);

    // 解密数据
    for (int i = 0; i < encryptedData.length(); i += blockSize)
    {
        AES_decrypt((const unsigned char*)(encryptedData.constData() + i),
                    (unsigned char*)(decryptedData.data() + i),
                    &aesKey);
    }

    // 去除填充的字节
    QDataStream stream(&decryptedData, QIODevice::ReadWrite);
    qint32 dataSize = 0;
    stream >> dataSize;
    decryptedData = decryptedData.mid(sizeof(qint32), dataSize);

    return decryptedData;
}