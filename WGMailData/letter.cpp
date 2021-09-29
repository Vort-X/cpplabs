#include "entities.h"

using namespace Entities;

Letter::Letter(int id,
               const QDateTime &sendingDateTime,
               const QString &receiver,
               const QString &sender,
               const QString &header,
               const QString &text)
{
    setId(id);
    setSendingDateTime(sendingDateTime);
    setReceiver(receiver);
    setSender(sender);
    setHeader(header);
    setText(text);
}

int Letter::getId() const
{
    return id;
}

QDateTime Letter::getSendingDateTime() const
{
    return sendingDateTime;
}

QString Letter::getReceiver() const
{
    return receiver;
}

QString Letter::getSender() const
{
    return sender;
}

QString Letter::getHeader() const
{
    return header;
}

QString Letter::getText() const
{
    return text;
}

void Letter::setId(int id)
{
    if (id > 0)
        this->id = id;
    else
        this->id = 0;
}

void Letter::setSendingDateTime(const QDateTime &sendingDateTime)
{
    this->sendingDateTime = sendingDateTime;
}

void Letter::setReceiver(const QString &reciever)
{
    this->receiver = reciever;
}

void Letter::setSender(const QString &sender)
{
    this->sender = sender;
}

void Letter::setHeader(const QString &header)
{
    this->header = header;
}

void Letter::setText(const QString &text)
{
    this->text = text;
}
