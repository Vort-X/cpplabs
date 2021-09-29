#ifndef ENTITIES_H
#define ENTITIES_H

#include <QDateTime>
#include <QString>
#include <QMap>

#include "WGMailData_global.h"

namespace Entities {

static const QString &k_userTableName = "user_table";
//static const QMap<QString, QString> &kUserParams = QMap<QString, QString>();

static const QString &k_letterTableName = "letter_table";
//static const QMap<QString, QString> &kLetterParams = QMap<QString, QString>();

//USERS table entity
class WGMAILDATA_EXPORT User
{
public:
    User(const QString &emailAddress,
         const QString &password,
         const QDateTime &registrationDateTime);

    QString getEmailAddress() const;
    QString getPassword() const;
    QDateTime getRegistrationDateTime() const;
    void setEmailAddress(const QString &emailAddress);
    void setPassword(const QString &password);
    void setRegistrationDateTime(const QDateTime &registrationDateTime);
private:
    QString emailAddress; //PRIMARY KEY
    QString password;
    QDateTime registrationDateTime;
};

//LETTERS table entity
class WGMAILDATA_EXPORT Letter
{
public:
    Letter(int id,
           const QDateTime &sendingDateTime,
           const QString &reciever,
           const QString &sender,
           const QString &header,
           const QString &text);

    int getId() const;
    QDateTime getSendingDateTime() const;
    QString getReceiver() const;
    QString getSender() const;
    QString getHeader() const;
    QString getText() const;
    void setId(int id);
    void setSendingDateTime(const QDateTime &sendingDateTime);
    void setReceiver(const QString &reciever);
    void setSender(const QString &sender);
    void setHeader(const QString &header);
    void setText(const QString &text);
private:
    int id; //PRIMARY KEY
    QDateTime sendingDateTime;
    QString receiver; //FOREIGN KEY
    QString sender; //FOREIGN KEY
    QString header;
    QString text;
};

}

#endif // ENTITIES_H
