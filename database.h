#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class Database
{
private:
    QSqlDatabase DATABASE;
public:
    Database();
    ~Database();
    void setFile(const QString & DATABASE_NAME);
    void open();
    void setPassword(const QString & PASSWORD);
    bool isOpen() const;
    void close();
    QSqlQuery query() const;
    void error() const;
};

#endif // DATABASE_H
