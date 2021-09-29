#include "entities.h"

using namespace Entities;

User::User(const QString &emailAddress,
           const QString &password,
           const QDateTime &registrationDateTime)
{
    setEmailAddress(emailAddress);
    setPassword(password);
    setRegistrationDateTime(registrationDateTime);
}

QString User::getEmailAddress() const
{
    return emailAddress;
}

QString User::getPassword() const
{
    return password;
}

QDateTime User::getRegistrationDateTime() const
{
    return registrationDateTime;
}

void User::setEmailAddress(const QString &emailAddress)
{
    this->emailAddress = emailAddress;
}

void User::setPassword(const QString &password)
{
    this->password = password;
}

void User::setRegistrationDateTime(const QDateTime &registrationDateTime)
{
    this->registrationDateTime = registrationDateTime;
}
