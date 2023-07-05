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

QString Database::md5(const QString & STRING) const
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(STRING.toUtf8());
    return hash.result().toHex();
}