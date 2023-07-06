#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Database & dbs, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), db(dbs)
{
    ui->setupUi(this);
    qDebug() << db.name();
    qDebug() << db.isOpen();
    showInfo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showInfo()
{
    ui->timenow->setText(db.getDate());
    QSqlQuery query = db.query();
    if (query.exec("SELECT name FROM user")) {
        while (query.next()) {
            QString value1 = query.value(0).toString();
            ui->username->setText(value1);
        }
    } else {
        qDebug() << "get user info failed:" << query.lastError().text();
    }
}