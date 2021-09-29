#include "emailserver.h"
#include "clientobject.h"
#include <QThread>
#include <functional>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTimer>
EmailServer::EmailServer(QObject *parent)
    : QTcpServer(parent)
{}

void EmailServer::incomingConnection(qintptr socketDescriptor) //new connection
{
    ClientObject *client = new ClientObject(this);
    if (!client->setSocketDescriptor(socketDescriptor))
    {
        client->deleteLater();
        return;
    }
    connect(client, &ClientObject::disconnectedFromClient, this, std::bind(&EmailServer::userDisconnected, this, client));
    connect(client, &ClientObject::error, this, std::bind(&EmailServer::userError, this, client));
    connect(client, &ClientObject::jsonReceived, this, std::bind(&EmailServer::jsonReceived, this, client, std::placeholders::_1));
    connect(client, &ClientObject::logMessage, this, &EmailServer::logMessage);
    m_clients.append(client);
    emit logMessage(QStringLiteral("New client Connected"));
}
void EmailServer::sendJson(ClientObject *destination, const QJsonObject &message) //send json to client, obviously
{
    Q_ASSERT(destination);
    destination->sendJson(message);
}

void EmailServer::jsonReceived(ClientObject *sender, const QJsonObject &request) //method called when client object recieves valid json
{
    Q_ASSERT(sender);
    emit logMessage(QLatin1String("JSON received ") + QString::fromUtf8(QJsonDocument(request).toJson()));
    QJsonObject response;
    const QString senderEmail = sender->getEmail();
    if (senderEmail.isEmpty())
    {
        response = m_processor->process(request);
        if (response[KEY_STATUS].toString() == VALUE_STATUS_OK)
            sender->setEmail(request[QStringLiteral("email")].toString());
    }
    else
    {
        response = m_processor->process(request, senderEmail);
    }
    sendJson(sender, response);
}

void EmailServer::userDisconnected(ClientObject *sender)
{
    m_clients.removeAll(sender);
    const QString email = sender->getEmail();
    if (!email.isEmpty())
    {
        emit logMessage(email + QLatin1String(" disconnected"));
    }
    sender->setEmail("");
    sender->deleteLater();
}

void EmailServer::userError(ClientObject *sender)
{
    Q_UNUSED(sender)
    emit logMessage(QLatin1String("Error from ") + sender->getEmail());
}

bool EmailServer::startServer(const QHostAddress &address, quint16 port)
{
    if(listen(address, port))
    {
        m_processor = new RequestProcessor(this);
        return true;
    }
    return false;
}

void EmailServer::stopServer()
{
    for (ClientObject *client : m_clients)
    {
        client->disconnectFromClient();
    }
    delete m_processor;
    close();
}
