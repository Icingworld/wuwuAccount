#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Database & dbs, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), db(dbs)
{
    ui->setupUi(this);
    ui->tabs->setTabText(0,"记录");
    ui->tabs->setTabText(1,"分析");
    ui->tabs->setCurrentIndex(0);
    showInfo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showInfo()
{
    ui->timenow->setText("当前：" + db.getDate());
    QSqlQuery query = db.query();
    if (query.exec("SELECT name FROM user")) {
        while (query.next()) {
            QString value1 = query.value(0).toString();
            ui->username->setText("用户名：" + value1);
        }
    } else {
        qDebug() << "get user info failed:" << query.lastError().text();
    }
}

void MainWindow::getRecord(const QDate & date)
{
    int year = date.year();
    int month = date.month();
    int day = date.day();
    QSqlQuery query = db.query();
    if (query.exec("SELECT name FROM user")) {
        while (query.next()) {

        }
    } else {
        qDebug() << "get account record failed:" << query.lastError().text();
    }
}

// void MainWindow::setCanlendar()
// {
    
// }