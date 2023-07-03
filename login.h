#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QDir>
#include <QDebug>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    void checkUser();

private:
    Ui::login *ui;
};

#endif // LOGIN_H
