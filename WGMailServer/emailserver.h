#ifndef EMAILSERVER_H
#define EMAILSERVER_H

#include <QTcpServer>
#include <QVector>
#include "requestprocessor.h"
class QThread;
class ClientObject;
class EmailServer : public QTcpServer
{
    Q_OBJECT

public:
    EmailServer(QObject *parent = nullptr);
protected:
    void incomingConnection(qintptr socketDescriptor) override;
signals:
    void logMessage(const QString &msg); //connected in StartWindow
public slots:
    bool startServer(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);
    void stopServer();
private slots:
    void jsonReceived(ClientObject *sender, const QJsonObject &request);
    void userDisconnected(ClientObject *sender);
    void userError(ClientObject *sender);
private:
    void sendJson(ClientObject *destination, const QJsonObject &message);
    QVector<ClientObject *> m_clients;
    RequestProcessor *m_processor;
};

#endif // EMAILSERVER_H
