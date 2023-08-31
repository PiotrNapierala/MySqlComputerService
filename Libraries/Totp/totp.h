#ifndef TOTP_H
#define TOTP_H

#include <QObject>
#include <QDebug>
#include <QRandomGenerator>

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using std::uint8_t;
using std::uint32_t;
using std::int64_t;
using std::uint64_t;
using std::size_t;
using std::string;
using std::vector;

class TOTP : public QObject
{
    Q_OBJECT
public:
    QString GetTOTPFromToken(QString token);
    QString GenerateRandomBase32Token(int tokenLength);

};

#endif // TOTP_H
