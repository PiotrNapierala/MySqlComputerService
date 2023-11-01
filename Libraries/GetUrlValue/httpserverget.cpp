#include "httpserverget.h"

HttpServerGet::HttpServerGet(int port) : m_port(port)
{
    m_server = new QTcpServer(this);
    QObject::connect(m_server, &QTcpServer::newConnection, this, &HttpServerGet::handleNewConnection);
}

void HttpServerGet::listen()
{
    if (m_server->listen(QHostAddress::Any, m_port)) {
        qDebug() << "Server started. Listening on port" << m_port;
    } else {
        qWarning() << "Server could not start!";
    }
}

void HttpServerGet::handleNewConnection()
{
    while (m_server->hasPendingConnections()) {
        QTcpSocket* clientSocket = m_server->nextPendingConnection();
        QObject::connect(clientSocket, &QTcpSocket::readyRead, this, &HttpServerGet::handleRequest);
    }
}

void HttpServerGet::handleRequest()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        QByteArray requestData = clientSocket->readAll();
        QUrl url = QUrl::fromEncoded(requestData);

        // Separate the query part from the URL
        QString queryPart = url.query();
        QUrlQuery query(queryPart);

        if (query.hasQueryItem("id")) {
            QString idValue = query.queryItemValue("id");
            QStringList values = idValue.split(" ");
            if(!values.isEmpty())
            {
                emit idReceived(values.first());
            }
        }

        // Close the connection
        clientSocket->close();
        clientSocket->deleteLater();
    }
}
