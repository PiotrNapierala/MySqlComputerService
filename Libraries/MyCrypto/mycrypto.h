
#ifndef MYCRYPTO_H
#define MYCRYPTO_H

#include <QObject>
#include <QCryptographicHash>
#include <QDebug>

class MyCrypto : public QObject
{
    Q_OBJECT
public:
    explicit MyCrypto(QString password, QCryptographicHash::Algorithm algorithm, QObject *parent = nullptr);
    QString EncryptData(QString data);
    QString DecryptData(QString data);
    void ChangePassword(QString password);
    void ChangeAlgorithm(QCryptographicHash::Algorithm algorithm);
    QString HashString(QString data, QCryptographicHash::Algorithm algorithm);
    QString GetKey();

private:
    int HexToInt(QString hexCharacter);
    QString MixData(QString data);
    QString ReverseDataMix(QString data);


    QCryptographicHash::Algorithm cryptoAlgorithm = QCryptographicHash::Sha256;
    QString cryptoPassword;
    QString cryptoKey;

signals:

};

#endif // MYCRYPTO_H
