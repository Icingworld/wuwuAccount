#include "database.h"

Database::Database()
{
    DATABASE = QSqlDatabase::addDatabase("QSQLITE");
}

Database::~Database()
{
    if (DATABASE.isOpen())
    {
        close();
    }
}

void Database::setFile(const QString & DATABASE_NAME)
{
    NAME = DATABASE_NAME;
    DATABASE.setDatabaseName(DATABASE_NAME);
}

void Database::open()
{
    if (DATABASE.open())
    {
        qDebug() << "open database successfully";
    } else {
        qDebug() << "failed to open database";
    }
}

void Database::setPassword(const QString & PASSWORD)
{
    DATABASE.setPassword(PASSWORD);
}

bool Database::isOpen() const
{
    return DATABASE.isOpen();
}

void Database::close()
{
    DATABASE.close();
}

QSqlQuery Database::query() const
{
    QSqlQuery QUERY(DATABASE);
    return QUERY;
}

void Database::error() const
{
    qDebug() << DATABASE.lastError().text();
}

QString Database::name() const
{
    return NAME;
}

QString Database::md5(const QString & STRING) const
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(STRING.toUtf8());
    return hash.result().toHex();
}

void Database::createUser(const QString & name, const QString & password)
{
    QString PASSWORD = md5(password);
    QSqlQuery query_ = query();
    query_.prepare("INSERT INTO user (name, password) VALUES (:name, :password)");
    query_.bindValue(":name", name);
    query_.bindValue(":password", PASSWORD);
    query_.exec();
}

QDate Database::getDate() const
{
    return QDate::currentDate();
}

/* set time of the date choosen by calendar */
void Database::setCurrentDate(QDate currentDate)
{
    date = currentDate;
}

QDate Database::currentDate() const
{
    return date;
}
