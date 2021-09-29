#include "repositories.h"

using namespace Repositories;

UserRepository::UserRepository(QSqlDatabase* db)
{
    this->db = db;
    if (!db->isOpen())
        db->open();
    QSqlQuery query;
    QString queryString = k_createTableScript.arg(
                k_userTableName,
                "email TEXT PRIMARY KEY NOT NULL, "
                "password TEXT, "
                "reg_date_time TEXT"
                );
    query.exec(queryString);
}

UserRepository::~UserRepository(){}

void UserRepository::Add(User* entity)
{
    if (!db->isOpen())
        db->open();
    QSqlQuery query;
    QString queryString = k_insertScript.arg(
                k_userTableName,
                "email, password, reg_date_time",
                QString("\"%1\", \"%2\", \"%3\"").arg(
                    entity->getEmailAddress(),
                    entity->getPassword(),
                    entity->getRegistrationDateTime().toString()
                    )
                );
    query.exec(queryString);
}

User* UserRepository::Find(const QString &pk)
{
    if (!db->isOpen())
        db->open();
    QSqlQuery query;
    QString queryString =k_selectScript.arg(
                "email, password, reg_date_time",
                k_userTableName,
                QString("email=\"%1\"").arg(pk)
                );
    query.exec(queryString);
    query.next();
    return new User(query.value(0).toString(), query.value(1).toString(), QDateTime::fromString(query.value(3).toString()));
}

QVector<User*> UserRepository::FindAll()
{
    if (!db->isOpen())
        db->open();
    QSqlQuery query;
    QString queryString = k_multiSelectScript.arg(
                "email, password, reg_date_time",
                k_userTableName
                );
    query.exec(queryString);
    QVector<User*> users;
    while(query.next())
    {
        users.append(new User(query.value(0).toString(),
                              query.value(1).toString(),
                              QDateTime::fromString(query.value(2).toString())));
    }
    return users;
}

void UserRepository::Update(User* entity)
{
    if (!db->isOpen())
        db->open();
    QSqlQuery query;
    QString queryString = k_updateScript.arg(
                k_userTableName,
                "password",
                "\""+entity->getPassword()+"\"",
                QString("email=\"%1\"").arg(entity->getEmailAddress())
                );
    query.exec(queryString);
}

void UserRepository::Remove(User* entity)
{
    if (!db->isOpen())
        db->open();
    QSqlQuery query;
    QString queryString =k_deleteScript.arg(
                k_userTableName,
                QString("email=\"%1\"").arg(entity->getEmailAddress())
                );
    query.exec(queryString);
}
