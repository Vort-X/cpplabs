#include "requestprocessor.h"
#include "unitofwork.h"

RequestProcessor::RequestProcessor(QObject *parent) : QObject(parent),
    m_uow(new UnitOfWork())
{}

RequestProcessor::~RequestProcessor()
{
    delete m_uow;
}

QJsonObject RequestProcessor::process(QJsonObject request)
{
    QJsonObject response;
    if (!isValidValue(KEY_TYPE, request, response))
        return response;
    QJsonValue typeVal = request[KEY_TYPE];
    response[KEY_TYPE] = typeVal;
    QString type = typeVal.toString();

    if (type == VALUE_TYPE_LOGIN)
    {
        if (!isValidValue(KEY_EMAIL, request, response))
            return response;
        const QString email = request[KEY_EMAIL].toString();

        if (!isValidValue(KEY_PASSWORD, request, response))
            return response;
        const QString password = request[KEY_PASSWORD].toString();

        User *userFromDb = m_uow->getUserRepository()->Find(email);
        QString encryptedPassword = encryptPassword(password);
        if (userFromDb->getEmailAddress().compare("") == 0 || encryptedPassword != userFromDb->getPassword())
        {
            response[KEY_STATUS] = VALUE_STATUS_FAIL;
            response[KEY_REASON] = "invalid email or password";
            return response;
        }

        response[KEY_STATUS] = VALUE_STATUS_OK;
        delete userFromDb;
        return response;
    }

    if (type == VALUE_TYPE_REGISTER)
    {
        if (!isValidValue(KEY_EMAIL, request, response))
            return response;
        const QString email = request[KEY_EMAIL].toString();

        if (m_uow->getUserRepository()->Find(email)->getEmailAddress().compare("") != 0)
        {
            response[KEY_STATUS] = VALUE_STATUS_FAIL;
            response[KEY_REASON] = "user with specified email exists";
            return response;
        }

        if (!validateMail(email))
        {
            response[KEY_STATUS] = VALUE_STATUS_FAIL;
            response[KEY_REASON] = "invalid email";
            return response;
        }

        if (!isValidValue(KEY_PASSWORD, request, response))
            return response;
        const QString password = request[KEY_PASSWORD].toString();

        for (QChar ch : password)
        {
            if (!LEGAL_SYMBOLS.contains(ch)) //A-Za-z0-9!@#$%^&*()+=~_- are legal
            {
                response[KEY_STATUS] = VALUE_STATUS_FAIL;
                response[KEY_REASON] = "invalid password";
                return response;
            }
        }

        QString encryptedPassword = encryptPassword(password);
        User newUser = User(email, encryptedPassword, QDateTime::currentDateTime());
        m_uow->getUserRepository()->Add(&newUser);
        response[KEY_STATUS] = VALUE_STATUS_OK;
        return response;
    }

    response[KEY_STATUS] = VALUE_STATUS_FAIL;
    response[KEY_REASON] = "invalid request type";
    return response;
}

QJsonObject RequestProcessor::process(QJsonObject request, QString senderEmail)
{
    QJsonObject response;
    if (!isValidValue(KEY_TYPE, request, response))
        return response;
    QJsonValue typeVal = request[KEY_TYPE];
    response[KEY_TYPE] = typeVal;
    QString type = typeVal.toString();

    if (type == VALUE_TYPE_GET_RECEIVED_ALL)
    {
//        response[KEY_STATUS] = VALUE_STATUS_FAIL;
//        response[KEY_REASON] = "not supported yet";
//        return response;

        response[KEY_STATUS] = VALUE_STATUS_OK;
        QJsonArray lettersArray = QJsonArray();
        QVector<Letter*> lettersFromDb = m_uow->getLetterRepository()->FindByReceiver(senderEmail);
        foreach (Letter* letter, lettersFromDb)
        {
            QJsonObject letterObject = QJsonObject();
            letterObject[KEY_ID] = letter->getId();
            letterObject[KEY_DATETIME] = letter->getSendingDateTime().toString();
            letterObject[KEY_SENDER] = letter->getSender();
            letterObject[KEY_HEADER] = letter->getHeader();
            letterObject[KEY_TEXT] = letter->getText();
            lettersArray.append(letterObject);
            delete letter;
        }
        response[KEY_LETTERS_LIST] = lettersArray;
        return response;
    }

    if (type == VALUE_TYPE_GET_RECEIVED_ONE)
    {
        if (!isValidValue(KEY_ID, request, response))
            return response;

        Letter *letterFromDb = m_uow->getLetterRepository()->Find(request[KEY_ID].toString().toInt());
        if (letterFromDb->getId() == 0 || letterFromDb->getReceiver() != senderEmail)
        {
            response[KEY_STATUS] = VALUE_STATUS_FAIL;
            response[KEY_REASON] = "no letter with specified id";
            return response;
        }

        response[KEY_STATUS] = VALUE_STATUS_OK;
        response[KEY_ID] = letterFromDb->getId();
        response[KEY_DATETIME] = letterFromDb->getSendingDateTime().toString();
        response[KEY_SENDER] = letterFromDb->getSender();
        response[KEY_HEADER] = letterFromDb->getHeader();
        response[KEY_TEXT] = letterFromDb->getText();
        delete letterFromDb;
        return response;
    }

    if (type == VALUE_TYPE_SEND)
    {
        if (!isValidValue(KEY_RECEIVER, request, response))
            return response;
        if (!isValidValue(KEY_HEADER, request, response))
            return response;
        if (!isValidValue(KEY_TEXT, request, response))
            return response;

        if (m_uow->getUserRepository()->Find(request[KEY_RECEIVER].toString())->getEmailAddress() == request[KEY_RECEIVER].toString())
        {
            Letter newLetter = Letter(0,
                                      QDateTime::currentDateTime(),
                                      request[KEY_RECEIVER].toString(),
                                      senderEmail,
                                      request[KEY_HEADER].toString(),
                                      request[KEY_TEXT].toString()
                                      );
            m_uow->getLetterRepository()->Add(&newLetter);
        }

        response[KEY_STATUS] = VALUE_STATUS_OK;
        return response;
    }

    if (type == VALUE_TYPE_DELETE)
    {
        if (!isValidValue(KEY_ID, request, response))
            return response;

        Letter *letterFromDb = m_uow->getLetterRepository()->Find(request[KEY_ID].toString().toInt());
        if (letterFromDb->getId() == 0 || letterFromDb->getReceiver() != senderEmail)
        {
            response[KEY_STATUS] = VALUE_STATUS_FAIL;
            response[KEY_REASON] = "no letter with specified id";
            return response;
        }

        m_uow->getLetterRepository()->Remove(letterFromDb);
        response[KEY_STATUS] = VALUE_STATUS_OK;
        delete letterFromDb;
        return response;
    }

    response[KEY_STATUS] = VALUE_STATUS_FAIL;
    response[KEY_REASON] = "invalid request type";
    return response;
}

QString RequestProcessor::encryptPassword(const QString &plainPassword)
{
    return plainPassword;
}

bool RequestProcessor::isValidValue(const QString &key, const QJsonObject &request, const QJsonObject &response) //remake
{
    QJsonValue value = request[key];
    if (value.isNull() || !value.isString())
    {
        response[KEY_STATUS] = VALUE_STATUS_FAIL;
        response[KEY_REASON] = "\"" + key + "\" value error";
        return false;
    }
    return true;
}

bool RequestProcessor::validateMail(const QString &email)
{
    if (email.isNull() || email.isEmpty())
        return false;
    if (!email.endsWith("@wg.com"))
        return false;
    return true;
}
