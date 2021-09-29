#include "unitofwork.h"

UnitOfWork::UnitOfWork()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("wgmaildata.sqlite");
    letterRepository = new LetterRepository(&db);
    userRepository = new UserRepository(&db);
}

UnitOfWork::~UnitOfWork()
{
    db.close();
    delete letterRepository;
    delete userRepository;
}

LetterRepository* UnitOfWork::getLetterRepository()
{
    return letterRepository;
}

UserRepository* UnitOfWork::getUserRepository()
{
    return userRepository;
}
