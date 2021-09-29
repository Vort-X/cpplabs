#ifndef REPOSITORIES_H
#define REPOSITORIES_H

#include <QVector>
#include <QtSql>
//#include "WGMailData_global.h"
#include "entities.h"

using namespace Entities;

namespace Repositories {


static const QString &k_createTableScript = "CREATE TABLE IF NOT EXISTS %1 (%2);"; //1-table 2-columns+types
static const QString &k_insertScript = "INSERT INTO %1 (%2) VALUES (%3);"; //1-table 2-columns 3-values
static const QString &k_updateScript = "UPDATE %1 SET %2=%3 WHERE %4;"; //1-table 2-column 3-value 4-expr
static const QString &k_deleteScript = "DELETE FROM %1 WHERE %2;"; //1-table 2-expr
static const QString &k_selectScript = "SELECT %1 FROM %2 WHERE %3;"; //1-columns 2-table 3-expr
static const QString &k_multiSelectScript = "SELECT %1 FROM %2;"; //1-columns 2-table

class WGMAILDATA_EXPORT UserRepository
{
public:
    UserRepository(QSqlDatabase* db);
    ~UserRepository();

    void Add(User* entity);
    User* Find(const QString &pk);
    QVector<User*> FindAll();
    void Update(User* entity);
    void Remove(User* entity);

private:
    QSqlDatabase* db;
};

class WGMAILDATA_EXPORT LetterRepository
{
public:
    LetterRepository(QSqlDatabase* db);
    ~LetterRepository();

    void Add(Letter* entity);
    Letter* Find(int pk);
    QVector<Letter*> FindAll();
    QVector<Letter*> FindByReceiver(const QString &reciever_fk);
    QVector<Letter*> FindBySender(const QString &sender_fk);
    void Update(Letter* entity);
    void Remove(Letter* entity);

private:
    QSqlDatabase* db;
};

}

#endif // REPOSITORIES_H
