#include "mycrypto.h"

MyCrypto::MyCrypto(QString password, QCryptographicHash::Algorithm algorithm, QObject *parent) : QObject{parent}
{
    cryptoPassword = password;
    cryptoAlgorithm = algorithm;
    cryptoKey = HashString(password, cryptoAlgorithm);
}

QString MyCrypto::EncryptData(QString data)
{
    if(data.isEmpty()) return "";

    QString encryptedData = data;

    QString ASCII = "!#$%&()*+-0123456789<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_abcdefghijklmnopqrstuvwxyz{|}~";

    QString currentKey = cryptoKey;
    while(currentKey.length() < encryptedData.length()) currentKey += cryptoKey;
    currentKey += cryptoKey;

    QString currentASCII = ASCII;
    while(currentASCII.length() < encryptedData.length()) currentASCII += ASCII;
    currentASCII += currentASCII;

    for(int i = 0; i < encryptedData.length(); i++)
    {
        QChar currentChar = encryptedData[i];
        if(currentChar != ' ')
        {
            if(ASCII.contains(currentChar))
            {
                int newCharPos = HexToInt(currentKey[i]) + ASCII.indexOf(currentChar);
                encryptedData[i] = currentASCII[newCharPos];
            }
        }
    }

    if(encryptedData.length() > 1) return MixData(encryptedData);
    else return encryptedData;
}

QString MyCrypto::DecryptData(QString data)
{
    if(data.isEmpty()) return "";

    QString decryptedData;

    if(data.length() > 1) decryptedData = ReverseDataMix(data);
    else decryptedData = data;

    QString ASCII = "!#$%&()*+-0123456789<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_abcdefghijklmnopqrstuvwxyz{|}~";

    QString currentKey = cryptoKey;
    while(currentKey.length() < decryptedData.length()) currentKey += cryptoKey;
    currentKey += cryptoKey;

    QString currentASCII = ASCII;
    while(currentASCII.length() < decryptedData.length()) currentASCII += ASCII;
    currentASCII += currentASCII;

    for(int i = decryptedData.length()-1; i >= 0; i--)
    {
        QChar currentChar = decryptedData[i];
        if(currentChar != ' ')
        {
            if(ASCII.contains(currentChar))
            {
                int newCharPos = ASCII.indexOf(currentChar) - HexToInt(currentKey[i]);
                if(newCharPos < 0) newCharPos = ASCII.length() - (newCharPos * -1);
                decryptedData[i] = currentASCII[newCharPos];
            }
        }
    }

    return decryptedData;
}

void MyCrypto::ChangePassword(QString password)
{
    cryptoPassword = password;
    cryptoKey = HashString(password, cryptoAlgorithm);
}

void MyCrypto::ChangeAlgorithm(QCryptographicHash::Algorithm algorithm)
{
    cryptoAlgorithm = algorithm;
    cryptoKey = HashString(cryptoPassword, algorithm);
}

QString MyCrypto::GetKey()
{
    return cryptoKey;
}

QString MyCrypto::HashString(QString data, QCryptographicHash::Algorithm algorithm)
{
    return QString(QCryptographicHash::hash(data.toUtf8(), algorithm).toHex());
}

int MyCrypto::HexToInt(QString hexCharacter)
{
    bool ok;
    int value = hexCharacter.toInt(&ok, 16);
    if(ok) return value;
    else return 1;
}

QString MyCrypto::MixData(QString data)
{
    QString mix = data;

    QString currentKey = cryptoKey;
    while(currentKey.length() < mix.length()) currentKey += cryptoKey;
    currentKey += cryptoKey;

    for(int i = 0; i < mix.length(); i++)
    {
        int index = i + HexToInt(currentKey[i]);
        if(index >= mix.length()) index = index % mix.length();

        QChar currentChar = mix[i];
        QChar newChar = mix[index];

        mix[i] = newChar;
        mix[index] = currentChar;
    }

    return mix;
}

QString MyCrypto::ReverseDataMix(QString data)
{
    QString mix = data;

    QString currentKey = cryptoKey;
    while(currentKey.length() < mix.length()) currentKey += cryptoKey;
    currentKey += cryptoKey;

    for(int i = mix.length()-1; i >= 0; i--)
    {
        int index = i + HexToInt(currentKey[i]);
        if(index >= mix.length()) index = index % mix.length();

        QChar currentChar = mix[i];
        QChar newChar = mix[index];

        mix[i] = newChar;
        mix[index] = currentChar;
    }

    return mix;
}

