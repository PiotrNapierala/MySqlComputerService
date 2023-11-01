#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QTcpSocket>

class HttpServerGet : public QObject
{
    Q_OBJECT

public:
    HttpServerGet(int port);

    void listen();

signals:
    void idReceived(const QString& id);

private slots:
    void handleNewConnection();
    void handleRequest();

private:
    QTcpServer* m_server;
    int m_port;
};

#endif // HTTPSERVER_H
