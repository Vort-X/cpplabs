#ifndef CLIENTOBJECT_H
#define CLIENTOBJECT_H

#include <QObject>
#include <QTcpSocket>
class QJsonObject;
class ClientObject : public QObject
{
    Q_OBJECT

public:
    ClientObject(QObject *parent = nullptr);
    virtual bool setSocketDescriptor(qintptr socketDescriptor);
    QString getEmail() const;
    void setEmail(const QString &userName);
    void sendJson(const QJsonObject &jsonData);
signals:
    void jsonReceived(const QJsonObject &jsonDoc);  //connected in EmailServer
    void disconnectedFromClient();                  //connected in EmailServer
    void error();                                   //connected in EmailServer
    void logMessage(const QString &msg);            //connected in EmailServer
public slots:
    void disconnectFromClient();
private slots:
    void receiveJson();
private:
    QTcpSocket *m_serverSocket;
    QString m_email;
};

#endif // CLIENTOBJECT_H
