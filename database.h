#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileDialog>
#include <QDate>
#include <QDebug>
#include <QCryptographicHash>
#include <QDateTime>

class Database
{
private:
    QSqlDatabase DATABASE;
    QString NAME;
    QDate date;
public:
    Database();
    ~Database();
    void setFile(const QString & DATABASE_NAME);
    void open();
    void setPassword(const QString & PASSWORD);
    bool isOpen() const;
    void close();
    QString name() const;
    QSqlQuery query() const;
    void error() const;
    QString md5(const QString & STRING) const;
    void createUser(const QString & name, const QString & password);
    QDate getDate() const;
    void setCurrentDate(QDate currentDate);
    QDate currentDate() const;
};

#endif // DATABASE_H
