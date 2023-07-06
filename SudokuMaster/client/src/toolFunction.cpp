/**
  ******************************************************************************
  * @file           : toolFunction.cpp
  * @author         : hui
  * @brief          : None
  * @attention      : None
  * @date           : 2023/7/6
  ******************************************************************************
  */

#include "toolFunction.h"
#include <openssl/evp.h>

namespace toolFunction {
    QByteArray encrypt(const QByteArray &key, const QByteArray &iv, const QByteArray &data) {
        // 创建一个加密上下文结构体
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        // 指定加密算法、模式和密钥长度
        EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, NULL, NULL);

        // 指定加密的密钥和初始向量
        EVP_EncryptInit_ex(ctx, NULL, NULL, reinterpret_cast<const unsigned char *>(key.constData()),
                           reinterpret_cast<const unsigned char *>(iv.constData()));
        // 对明文进行加密，并且输出到密文缓冲区
        int len;
        int ciphertext_len;
        QByteArray encryptedData;
        encryptedData.resize(data.size() + AES_BLOCK_SIZE); // 预留足够的空间
        EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char *>(encryptedData.data()), &len,
                          reinterpret_cast<const unsigned char *>(data.constData()), data.size());
        ciphertext_len = len;
        // 对最后一块明文进行加密，并且输出到密文缓冲区
        EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(encryptedData.data()) + len, &len);
        ciphertext_len += len;
        // 释放加密上下文结构体
        EVP_CIPHER_CTX_free(ctx);
        // 调整密文缓冲区的大小
        encryptedData.resize(ciphertext_len);
        // 返回密文
        return encryptedData;
    }

    QByteArray decrypt(const QByteArray &key, const QByteArray &iv, const QByteArray &encryptedData) {
        // 创建一个解密上下文结构体
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        // 指定解密算法、模式和密钥长度
        EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, NULL, NULL);
        // 指定解密的密钥和初始向量
        EVP_DecryptInit_ex(ctx, NULL, NULL, reinterpret_cast<const unsigned char *>(key.constData()),
                           reinterpret_cast<const unsigned char *>(iv.constData()));
        // 对密文进行解密，并且输出到明文缓冲区
        int len;
        int plaintext_len;
        QByteArray data;
        data.resize(encryptedData.size()); // 预留足够的空间
        EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char *>(data.data()), &len,
                          reinterpret_cast<const unsigned char *>(encryptedData.constData()), encryptedData.size());
        plaintext_len = len;
        // 对最后一块密文进行解密，并且输出到明文缓冲区
        EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(data.data()) + len, &len);
        plaintext_len += len;
        // 释放解密上下文结构体
        EVP_CIPHER_CTX_free(ctx);
        // 调整明文缓冲区的大小
        data.resize(plaintext_len);
        // 返回明文
        return data;
    }
} // toolFunction