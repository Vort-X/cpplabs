#include "clientobject.h"
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

ClientObject::ClientObject(QObject *parent)
    : QObject(parent)
    , m_serverSocket(new QTcpSocket(this))
{
    connect(m_serverSocket, &QTcpSocket::readyRead, this, &ClientObject::receiveJson);
    connect(m_serverSocket, &QTcpSocket::disconnected, this, &ClientObject::disconnectedFromClient);
    connect(m_serverSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &ClientObject::error);
}


bool ClientObject::setSocketDescriptor(qintptr socketDescriptor)
{
    return m_serverSocket->setSocketDescriptor(socketDescriptor);
}

void ClientObject::sendJson(const QJsonObject &json)
{
    const QByteArray jsonData = QJsonDocument(json).toJson(QJsonDocument::Compact);
    emit logMessage(QLatin1String("Sending to ") + getEmail() + QLatin1String(" - ") + QString::fromUtf8(jsonData));
    QDataStream socketStream(m_serverSocket);
    socketStream.setVersion(QDataStream::Qt_5_7);    //socketStream.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    socketStream << jsonData;
}

void ClientObject::disconnectFromClient()
{
    m_serverSocket->disconnectFromHost();
}

QString ClientObject::getEmail() const
{
    return m_email;
}

void ClientObject::setEmail(const QString &email)
{
    m_email = email;
}

void ClientObject::receiveJson()
{
    QByteArray jsonData;
    QDataStream socketStream(m_serverSocket);
    socketStream.setVersion(QDataStream::Qt_5_7);   //socketStream.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    for (;;)
    {
        socketStream.startTransaction();
        socketStream >> jsonData;
        if (socketStream.commitTransaction())
        {
            QJsonParseError parseError;
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
            if (parseError.error == QJsonParseError::NoError)
            {
                if (jsonDoc.isObject())
                    emit jsonReceived(jsonDoc.object());
                else
                    emit logMessage(QLatin1String("Invalid message: ") + QString::fromUtf8(jsonData));
            }
            else
            {
                emit logMessage(QLatin1String("Invalid message: ") + QString::fromUtf8(jsonData));
            }
        }
        else
        {
            break;
        }
    }
}
