#include "totp.h"

/*
 * Time-based One-Time Password tools (C++)
 *
 * Copyright (c) 2022 Project Nayuki. (MIT License)
 * https://www.nayuki.io/page/time-based-one-time-password-tools
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 *
 *   This is a modified version of the library for MySqlComputerService. The full version of the library can be found at:
 *   https://www.nayuki.io/page/time-based-one-time-password-tools
 */

static vector<uint8_t> decodeBase32(const char *str);
string calcTotp(
    vector<uint8_t> secretKey,
    int64_t epoch,
    int64_t timeStep,
    int64_t timestamp,
    int codeLen,
    vector<uint8_t> (*hashFunc)(vector<uint8_t>),
    int blockSize);
string calcHotp(vector<uint8_t> secretKey,
                const vector<uint8_t> &counter,
                int codeLen,
                vector<uint8_t> (*hashFunc)(vector<uint8_t>),
                int blockSize);
static vector<uint8_t> calcHmac(vector<uint8_t> key,
                                const vector<uint8_t> &message,
                                vector<uint8_t> (*hashFunc)(vector<uint8_t>),
                                int blockSize);
vector<uint8_t> calcSha1Hash(vector<uint8_t> message);
static vector<uint8_t> toBytesBigEndian(uint64_t x);
static uint32_t rotateLeft(uint32_t x, int i);

QString TOTP::GetTOTPFromToken(QString token)
{
    vector<uint8_t> secretKey = decodeBase32(token.toUtf8());
    int64_t timestamp = std::time(nullptr);
    string code = calcTotp(std::move(secretKey), 0, 30, timestamp, 6, calcSha1Hash, 64);
    return QString::fromStdString(code);
}

QString TOTP::GenerateRandomBase32Token(int tokenLength)
{
    static const QString base32Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    QString token;
    for (int i = 0; i < tokenLength; ++i) {
        int randomIndex = QRandomGenerator::global()->bounded(base32Alphabet.length());
        token += base32Alphabet[randomIndex];
    }
    return token;
}

static vector<uint8_t> decodeBase32(const char *str) {
    static const char *const ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    vector<uint8_t> result;
    for (int bits = 0, bitsLen = 0; *str != '\0'; str++) {
        char c = *str;
        if (c == ' ')
            continue;
        const char *p = std::strchr(ALPHABET, std::toupper(c));
        if (p == nullptr)
            throw std::domain_error("Invalid Base32 string");
        bits = (bits << 5) | (p - ALPHABET);
        bitsLen += 5;
        if (bitsLen >= 8) {
            bitsLen -= 8;
            result.push_back(static_cast<uint8_t>(bits >> bitsLen));
            bits &= (1 << bitsLen) - 1;
        }
    }
    return result;
}

string calcTotp(
    vector<uint8_t> secretKey,
    int64_t epoch,
    int64_t timeStep,
    int64_t timestamp,
    int codeLen,
    vector<uint8_t> (*hashFunc)(vector<uint8_t>),
    int blockSize) {

    int64_t temp = timestamp - epoch;
    if (temp < 0)
        temp -= timeStep - 1;
    uint64_t timeCounter = static_cast<uint64_t>(temp / timeStep);
    vector<uint8_t> counter = toBytesBigEndian(timeCounter);
    return calcHotp(std::move(secretKey), counter, codeLen, hashFunc, blockSize);
}

string calcHotp(
    vector<uint8_t> secretKey,
    const vector<uint8_t> &counter,
    int codeLen,
    vector<uint8_t> (*hashFunc)(vector<uint8_t>),
    int blockSize) {

    if (!(1 <= codeLen && codeLen <= 9))
        throw std::domain_error("Invalid number of digits");
    vector<uint8_t> hash = calcHmac(std::move(secretKey), counter, hashFunc, blockSize);

    int offset = hash.back() & 0xF;
    unsigned long val = 0;
    for (int i = 0; i < 4; i++)
        val |= static_cast<unsigned long>(hash.at(offset + i)) << ((3 - i) * 8);
    val &= 0x7FFFFFFFUL;

    unsigned long tenPow = 1;
    for (int i = 0; i < codeLen; i++)
        tenPow *= 10;
    std::ostringstream result;
    result << std::setw(codeLen) << std::setfill('0') << (val % tenPow);
    return result.str();
}

static vector<uint8_t> calcHmac(
    vector<uint8_t> key,
    const vector<uint8_t> &message,
    vector<uint8_t> (*hashFunc)(vector<uint8_t>),
    int blockSize) {

    if (blockSize < 1)
        throw std::domain_error("Invalid block size");

    if (key.size() > static_cast<unsigned int>(blockSize))
        key = hashFunc(key);
    while (key.size() < static_cast<unsigned int>(blockSize))
        key.push_back(0);

    vector<uint8_t> innerMsg;
    for (auto it = key.cbegin(); it != key.cend(); ++it)
        innerMsg.push_back(static_cast<uint8_t>(*it ^ 0x36));
    innerMsg.insert(innerMsg.end(), message.cbegin(), message.cend());
    vector<uint8_t> innerHash = hashFunc(std::move(innerMsg));

    vector<uint8_t> outerMsg;
    for (auto it = key.cbegin(); it != key.cend(); ++it)
        outerMsg.push_back(static_cast<uint8_t>(*it ^ 0x5C));
    outerMsg.insert(outerMsg.end(), innerHash.cbegin(), innerHash.cend());
    return hashFunc(std::move(outerMsg));
}

vector<uint8_t> calcSha1Hash(vector<uint8_t> message) {
    vector<uint8_t> bitLenBytes = toBytesBigEndian(message.size() * UINT64_C(8));
    message.push_back(0x80);
    while ((message.size() + 8) % 64 != 0)
        message.push_back(0x00);
    message.insert(message.end(), bitLenBytes.cbegin(), bitLenBytes.cend());

    uint32_t state[] = {
        UINT32_C(0x67452301),
        UINT32_C(0xEFCDAB89),
        UINT32_C(0x98BADCFE),
        UINT32_C(0x10325476),
        UINT32_C(0xC3D2E1F0),
    };
    for (size_t i = 0; i < message.size(); i += 64) {
        vector<uint32_t> schedule(16, 0);
        for (size_t j = 0; j < schedule.size() * 4; j++)
            schedule.at(j / 4) |= static_cast<uint32_t>(message.at(i + j)) << ((3 - j % 4) * 8);
        for (size_t j = schedule.size(); j < 80; j++) {
            uint32_t temp = schedule.at(j - 3) ^ schedule.at(j - 8) ^ schedule.at(j - 14) ^ schedule.at(j - 16);
            schedule.push_back(rotateLeft(temp, 1));
        }
        uint32_t a = state[0];
        uint32_t b = state[1];
        uint32_t c = state[2];
        uint32_t d = state[3];
        uint32_t e = state[4];
        for (size_t j = 0; j < schedule.size(); j++) {
            uint32_t f, rc;
            switch (j / 20) {
            case 0:  f = (b & c) | (~b & d);           rc = UINT32_C(0x5A827999);  break;
            case 1:  f = b ^ c ^ d;                    rc = UINT32_C(0x6ED9EBA1);  break;
            case 2:  f = (b & c) ^ (b & d) ^ (c & d);  rc = UINT32_C(0x8F1BBCDC);  break;
            case 3:  f = b ^ c ^ d;                    rc = UINT32_C(0xCA62C1D6);  break;
            default:  throw std::domain_error("Assertion error");
            }
            uint32_t temp = 0U + rotateLeft(a, 5) + f + e + schedule.at(j) + rc;
            e = d;
            d = c;
            c = rotateLeft(b, 30);
            b = a;
            a = temp;
        }
        state[0] = 0U + state[0] + a;
        state[1] = 0U + state[1] + b;
        state[2] = 0U + state[2] + c;
        state[3] = 0U + state[3] + d;
        state[4] = 0U + state[4] + e;
    }

    vector<uint8_t> result;
    for (uint32_t val : state) {
        for (int i = 3; i >= 0; i--)
            result.push_back(static_cast<uint8_t>(val >> (i * 8)));
    }
    return result;
}

static vector<uint8_t> toBytesBigEndian(uint64_t x) {
    vector<uint8_t> result(8);
    for (auto it = result.rbegin(); it != result.rend(); ++it, x >>= 8)
        *it = static_cast<uint8_t>(x);
    return result;
}

static uint32_t rotateLeft(uint32_t x, int i) {
    return ((0U + x) << i) | (x >> (32 - i));
}
