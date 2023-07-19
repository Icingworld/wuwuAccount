#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include "database.h"
#include "mainwindow.h"

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    void keyPressEvent(QKeyEvent *event);
    /* get all users exist */
    void checkUser();
    /* messagebox */
    void note(const QString & string);
    void newType(const QString & name, int mode, const QString & color);
    bool launchDatabase(const QString & name);
    void initDatabase(const QString & name, const QString & password);
    void launchMainWindow();
    QStringList users;
    Database db;
    MainWindow *mainwindow;

private slots:
    /* transfer to register mode */
    void on_toRegister_clicked();
    /* transfer to login mode */
    void on_toLogin_clicked();
    /* login to an existig account */
    void on_Login_clicked();
    /* register a new account */
    void on_Register_clicked();

private:
    Ui::login *ui;
};

#endif // LOGIN_H
