#ifndef REQUESTPROCESSOR_H
#define REQUESTPROCESSOR_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>

static const QString &LEGAL_SYMBOLS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()+=~_-";

#ifndef wg_json_localisation
#define wg_json_localisation
static const QString &KEY_DATETIME = "datetime";

static const QString &KEY_EMAIL = "email";

static const QString &KEY_HEADER = "header";

static const QString &KEY_ID = "id";

static const QString &KEY_LETTERS_LIST = "letters";

static const QString &KEY_PASSWORD = "password";

static const QString &KEY_REASON = "reason";

static const QString &KEY_RECEIVER = "receiver";

static const QString &KEY_SENDER = "sender";

static const QString &KEY_TEXT = "text";

static const QString &KEY_TYPE = "type";
static const QString &VALUE_TYPE_DELETE = "delete";
static const QString &VALUE_TYPE_GET_RECEIVED_ONE = "get received one";
static const QString &VALUE_TYPE_GET_RECEIVED_ALL = "get received all";
//static const QString &VALUE_TYPE_GET_SENT_ONE = "get sent one";
//static const QString &VALUE_TYPE_GET_SENT_ALL = "get sent all";
static const QString &VALUE_TYPE_LOGIN = "login";
//static const QString &VALUE_TYPE_RECEIVE = "receive";
static const QString &VALUE_TYPE_REGISTER = "register";
static const QString &VALUE_TYPE_SEND = "send";

static const QString &KEY_STATUS = "status";
static const QString &VALUE_STATUS_FAIL = "fail";
static const QString &VALUE_STATUS_OK = "ok";
#endif // wg_json_localisation

class UnitOfWork;
class RequestProcessor : public QObject
{
    Q_OBJECT
public:
    explicit RequestProcessor(QObject *parent = nullptr);
    ~RequestProcessor();

    QJsonObject process(QJsonObject request);
    QJsonObject process(QJsonObject request, QString senderEmail);

signals:
private:
    QString encryptPassword(const QString &plainPassword);
    bool isValidValue(const QString &key, const QJsonObject &request, const QJsonObject &response);
    bool validateMail(const QString &email);
    UnitOfWork *m_uow;
};

#endif // REQUESTPROCESSOR_H
