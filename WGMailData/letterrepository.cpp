#include "repositories.h"

using namespace Repositories;

LetterRepository::LetterRepository(QSqlDatabase* db)
{
    this->db = db;
    if (!db->isOpen())
        db->open();
    QSqlQuery query;
    QString queryString = k_createTableScript.arg(
                k_letterTableName,
                "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                "send_date_time TEXT, "
                "receiver TEXT, "
                "sender TEXT, "
                "header TEXT, "
                "text TEXT, "
                "FOREIGN KEY(receiver) REFERENCES " + k_userTableName + "(email), "
                "FOREIGN KEY(sender) REFERENCES " + k_userTableName + "(email)"
                );
    query.exec(queryString);
}

LetterRepository::~LetterRepository(){}

void LetterRepository::Add(Letter* entity)
{
    if (!db->isOpen())
        db->open();
    QSqlQuery query;
//    QString queryString = k_insertScript.arg(
//                k_letterTableName,
//                "id, send_date_time, receiver, sender, header, text",
//                QString("\"%1\", \"%2\", \"%3\", \"%4\", \"%5\", \"%6\"").arg(
//                    QString::number(entity->getId()),
//                    entity->getSendingDateTime().toString(),
//                    entity->getReciever(),
//                    entity->getSender(),
//                    entity->getHeader(),
//                    entity->getText()
//                    )
//                );
    QString queryString = k_insertScript.arg(
                k_letterTableName,
                "send_date_time, receiver, sender, header, text",
                QString("\"%1\", \"%2\", \"%3\", \"%4\", \"%5\"").arg(
                    entity->getSendingDateTime().toString(),
                    entity->getReceiver(),
                    entity->getSender(),
                    entity->getHeader(),
                    entity->getText()
                    )
                );
    //printf(queryString.toStdString().c_str());
    query.exec(queryString);
}

Letter* LetterRepository::Find(int pk)
{
    if (!db->isOpen())
        db->open();
    QSqlQuery query;
    QString queryString = k_selectScript.arg(
                "id, send_date_time, receiver, sender, header, text",
                k_letterTableName,
                QString("id=\"%1\"").arg(QString::number(pk))
                );
    query.exec(queryString);
    query.next();
    return new Letter(query.value(0).toInt(),
                      QDateTime::fromString(query.value(1).toString()),
                      query.value(2).toString(),
                      query.value(3).toString(),
                      query.value(4).toString(),
                      query.value(5).toString());
}

QVector<Letter*> LetterRepository::FindAll()
{
    if (!db->isOpen())
        db->open();
    QSqlQuery query;
    QString queryString = k_multiSelectScript.arg(
                "id, send_date_time, receiver, sender, header, text",
                k_letterTableName
                );
    query.exec(queryString);
    QVector<Letter*> letters;
    while(query.next())
    {
        letters.append(new Letter(query.value(0).toInt(),
                                  QDateTime::fromString(query.value(1).toString()),
                                  query.value(2).toString(),
                                  query.value(3).toString(),
                                  query.value(4).toString(),
                                  query.value(5).toString()));
    }
    return letters;
}

QVector<Letter*> LetterRepository::FindByReceiver(const QString &receiver_fk)
{
    if (!db->isOpen())
        db->open();
    QSqlQuery query;
    QString queryString = k_selectScript.arg(
                "id, send_date_time, receiver, sender, header, text",
                k_letterTableName,
                QString("receiver=\"%1\"").arg(receiver_fk)
                );
    query.exec(queryString);
    QVector<Letter*> letters;
    while(query.next())
    {
        letters.append(new Letter(query.value(0).toInt(),
                                  QDateTime::fromString(query.value(1).toString()),
                                  query.value(2).toString(),
                                  query.value(3).toString(),
                                  query.value(4).toString(),
                                  query.value(5).toString()));
    }
    return letters;
}

QVector<Letter*> LetterRepository::FindBySender(const QString &sender_fk)
{
    if (!db->isOpen())
        db->open();
    QSqlQuery query;
    QString queryString = k_selectScript.arg(
                "id, send_date_time, receiver, sender, header, text",
                k_letterTableName,
                QString("sender=\"%1\"").arg(sender_fk)
                );
    query.exec(queryString);
    QVector<Letter*> letters;
    while(query.next())
    {
        letters.append(new Letter(query.value(0).toInt(),
                                  QDateTime::fromString(query.value(1).toString()),
                                  query.value(2).toString(),
                                  query.value(3).toString(),
                                  query.value(4).toString(),
                                  query.value(5).toString()));
    }
    return letters;
}

void LetterRepository::Update(Letter* entity) //useless method
{
    if (!db->isOpen())
        db->open();
    QSqlQuery query;
    QString queryString = k_updateScript.arg(
                k_letterTableName,
                "header",
                "\""+entity->getHeader()+"\"",
                QString("id=\"%1\"").arg(QString::number(entity->getId()))
                );
    query.exec(queryString);
    queryString = k_updateScript.arg(
                k_letterTableName,
                "text",
                "\""+entity->getText()+"\"",
                QString("id=\"%1\"").arg(QString::number(entity->getId()))
                );
    query.exec(queryString);
}

void LetterRepository::Remove(Letter* entity)
{
    if (!db->isOpen())
        db->open();
    QSqlQuery query;
    QString queryString = k_deleteScript.arg(
                k_letterTableName,
                QString("id=\"%1\"").arg(QString::number(entity->getId()))
                );
    query.exec(queryString);
}
