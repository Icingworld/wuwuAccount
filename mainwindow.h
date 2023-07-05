#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QCryptographicHash>
#include <QDate>
#include <QFileDialog>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Database & dbs, QWidget *parent = nullptr);
    ~MainWindow();
    QString md5(const QString & str) const;
    void setPassword(const QString & PASSWORD);
    void launchDatabase();
    void initDatabase();
    QString getDate() const;
    void newType(const QString & name, int mode, const QString & color);
    void deleteType(const QString & name);
    void addAccount(const int & date, const QString & type, const double & amount, QString note);
    void createUser(const QString & name, const QString & password);
    void deleteUser(const QString & name, const QString & password);
    void import();


private:
    Ui::MainWindow *ui;
    Database db;
};
#endif // MAINWINDOW_H
