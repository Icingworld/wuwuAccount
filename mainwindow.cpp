#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Database & dbs, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), db(dbs)
{
    ui->setupUi(this);
    launchDatabase();
    initDatabase();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::md5(const QString & STRING) const
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(STRING.toUtf8());
    return hash.result().toHex();
}

void MainWindow::setPassword(const QString & PASSWORD)
{
    db.setPassword(PASSWORD);
}

void MainWindow::launchDatabase()
{
    db.setFile("test.db");
    db.open();
}

void MainWindow::initDatabase()
{
    QSqlQuery query = db.query();
    /* name: 名字 password: 密码的md5 */
    query.exec("CREATE TABLE IF NOT EXISTS user (id INTEGER PRIMARY KEY NOT NULL, name TEXT NOT NULL, password TEXT NOT NULL)");
    /* name: 名称 mode: 类别 color: 颜色 */
    query.exec("CREATE TABLE IF NOT EXISTS type (id INTEGER PRIMARY KEY NOT NULL, name TEXT NOT NULL, mode INTEGER NOT NULL, color TEXT NOT NULL)");
    /* date: 日期 type: 种类 amount: 数量 note: 备注 */
    query.exec("CREATE TABLE IF NOT EXISTS account (id INTEGER PRIMARY KEY NOT NULL, date INTEGER NOT NULL, type TEXT NOT NULL, amount REAL NOT NULL, note TEXT)");
    newType("工资", 0, "green");
    newType("外快", 0, "green");
    newType("其他", 0, "green");
    newType("饮食", 1, "red");
    newType("通讯", 1, "red");
    newType("交通", 1, "red");
    newType("娱乐", 1, "red");
    newType("房租", 1, "red");
    newType("水电", 1, "red");
    newType("教育", 1, "red");
    newType("医疗", 1, "red");
    newType("保险", 1, "red");
    newType("健身", 1, "red");
}

QString MainWindow::getDate() const
{
    QDate currentDate = QDate::currentDate();
    QString formattedDate = currentDate.toString("yyyy-MM-dd");
    return formattedDate;
}

void MainWindow::newType(const QString & name, int mode, const QString & color)
{
    QSqlQuery query = db.query();
    query.prepare("INSERT INTO type (name, mode, color) VALUES (:name, :mode, :color)");
    query.bindValue(":name", name);
    query.bindValue(":mode", mode);
    query.bindValue(":color", color);
    query.exec();
}

void MainWindow::deleteType(const QString & name)
{
    QSqlQuery query = db.query();
    query.prepare("DELETE FROM type WHERE name = :name");
    query.bindValue(":name", name);
    query.exec();
}

void MainWindow::addAccount(const int & date, const QString & type, const double & amount, QString note = "")
{
    QSqlQuery query = db.query();
    query.prepare("INSERT INTO account (date, type, amount, note) VALUES (:date, :type, :amount, :note)");
    query.bindValue(":date", date);
    query.bindValue(":type", type);
    query.bindValue(":amount", amount);
    query.bindValue(":note", note);
    query.exec();
}

void MainWindow::createUser(const QString & name, const QString & password)
{
    QString PASSWORD = md5(password);
    QSqlQuery query = db.query();
    query.prepare("INSERT INTO user (name, password) VALUES (:name, :password)");
    query.bindValue(":name", name);
    query.bindValue(":password", PASSWORD);
    query.exec();
}

void MainWindow::deleteUser(const QString & name, const QString & password)
{
    /* 删除用户文件夹 */
    qDebug() << name << password;
}

/* 导入原配置文件 */
void MainWindow::import()
{
    /* 选择文件 */
    QString Path = QFileDialog::getOpenFileName(this, QString("选择一个存档"), "/", "(*.db)");
    if (Path.isEmpty())
    {
        qDebug() << "No File Selected";
    }
}
