#ifndef UNITOFWORK_H
#define UNITOFWORK_H

#include <QtSql>
#include "repositories.h"
//#include "WGMailData_global.h"

using namespace Repositories;

class WGMAILDATA_EXPORT UnitOfWork
{
public:
    UnitOfWork();
    ~UnitOfWork();

    LetterRepository* getLetterRepository();
    UserRepository* getUserRepository();
private:
    LetterRepository* letterRepository;
    UserRepository* userRepository;
    QSqlDatabase db;
};

#endif // UNITOFWORK_H
