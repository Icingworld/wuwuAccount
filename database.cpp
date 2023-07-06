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


QString Database::getDate() const
{
    QDate currentDate = QDate::currentDate();
    QString formattedDate = currentDate.toString("yyyy.MM.dd");
    return formattedDate;
}

void Database::deleteType(const QString & name)
{
    QSqlQuery query_ = query();
    query_.prepare("DELETE FROM type WHERE name = :name");
    query_.bindValue(":name", name);
    query_.exec();
}

void Database::addAccount(const int & date, const QString & type, const double & amount, QString note = "")
{
    QSqlQuery query_ = query();
    query_.prepare("INSERT INTO account (date, type, amount, note) VALUES (:date, :type, :amount, :note)");
    query_.bindValue(":date", date);
    query_.bindValue(":type", type);
    query_.bindValue(":amount", amount);
    query_.bindValue(":note", note);
    query_.exec();
}

void Database::deleteUser(const QString & name, const QString & password)
{
    /* 删除用户文件夹 */
    qDebug() << name << password;
}

/* 导入原配置文件 */
void Database::import(QDialog & qdialog)
{
    /* 选择文件 */
    QString Path = QFileDialog::getOpenFileName(&qdialog, QString("选择一个存档"), "/", "(*.db)");
    if (Path.isEmpty())
    {
        qDebug() << "No File Selected";
    }
}

Qstring Database::getTime() const
{
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    return timestamp;
}