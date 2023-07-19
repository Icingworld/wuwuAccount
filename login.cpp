#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/logo.png"));
    // remove the help button -> '?'
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    checkUser();
    ui->password->setFocus();
}

login::~login()
{
    delete ui;
}

void login::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        ui->Login->click(); // 执行按钮的点击操作
    }
}

void login::checkUser()
{
    QDir dataDir("data");
    if (!dataDir.exists()) {
        // if not exists, create
        dataDir.mkpath(".");
        qDebug() << "Created 'data' folder.";
    } else {
        // get all users account
        QStringList user = dataDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString & fileName : user) {
            QFile file("data/" + fileName + "/" + fileName + ".db");
            if (file.exists())
            {
                users.append(fileName);
            }
        }
        ui->chooseAccount->addItems(users);
    }
}

void login::note(const QString & string)
{
    QMessageBox messageBox;
    messageBox.setWindowTitle("Warning");
    messageBox.setText(string);
    messageBox.setIcon(QMessageBox::Information);
    messageBox.addButton("确认", QMessageBox::AcceptRole);
    messageBox.exec();
}

bool login::launchDatabase(const QString & name)
{
    db.setFile(name);
    db.open();
    return db.isOpen();
}

void login::launchMainWindow()
{
    mainwindow = new MainWindow(db);
    mainwindow->show();
    QDialog::close();
}

void login::newType(const QString & name, int mode, const QString & color)
{
    QSqlQuery query = db.query();
    query.prepare("INSERT INTO type (name, mode, color) VALUES (:name, :mode, :color)");
    query.bindValue(":name", name);
    query.bindValue(":mode", mode);
    query.bindValue(":color", color);
    query.exec();
}

void login::initDatabase(const QString & name, const QString & password)
{
    QSqlQuery query = db.query();
    // name: 名字 password: 密码的md5
    query.exec("CREATE TABLE IF NOT EXISTS user (id INTEGER PRIMARY KEY NOT NULL, name TEXT NOT NULL, password TEXT NOT NULL)");
    db.createUser(name, password);
    // name: 名称 mode: 类别 color: 颜色
    query.exec("CREATE TABLE IF NOT EXISTS type (id INTEGER PRIMARY KEY NOT NULL, name TEXT NOT NULL, mode INTEGER NOT NULL, color TEXT NOT NULL)");
    // date: 日期 type: 种类 amount: 数量 note: 备注
    query.exec("CREATE TABLE IF NOT EXISTS account (id INTEGER PRIMARY KEY NOT NULL, mode INTEGER NOT NULL, type TEXT NOT NULL, amount REAL NOT NULL, year INTEGER NOT NULL, month INTEGER NOT NULL, day INTEGER NOT NULL, note TEXT)");
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
    newType("其他", 1, "red");
}

void login::on_toRegister_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_2->setCurrentIndex(1);
    ui->password->clear();
    ui->typeAccount->setFocus();
}

void login::on_toLogin_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->password->clear();
    ui->password->setFocus();
}

void login::on_Login_clicked()
{
    QString password = ui->password->text();
    if (password == "")
    {
        note("密码不能为空！");
        return;
    }
    if (ui->stackedWidget->currentIndex() == 0)
    {
        // login mode
        QString accountName = ui->chooseAccount->currentText();
        if (!launchDatabase("data/" + accountName + "/" + accountName + ".db"))
        {
            note("读取失败！");
        } else {
            // judge the password
            QString value{};
            QSqlQuery query = db.query();
            if (query.exec("SELECT password FROM user")) {
                while (query.next()) {
                    value = query.value(0).toString();
                }
            } else {
                qDebug() << "get user info failed:" << query.lastError().text();
            }
            if (value == db.md5(password))
            {
                // login and launch mainwindow
                launchMainWindow();
            } else {
                // password wrong
                note("密码错误！");
            }
        }
    } else {
        // register mode
        QString accountName = ui->typeAccount->text();
        if (!launchDatabase("data/" + accountName + "/" + accountName + ".db"))
        {
            note("读取失败！");
        } else {
            // init user info and database
            initDatabase(accountName, password);
            launchMainWindow();
        }
    }
}

void login::on_Register_clicked()
{
    QString accountName = ui->typeAccount->text();
    if (users.contains(accountName))
    {
        note("用户已存在！");
    } else {
        QDir dataDir("data/");
        dataDir.mkpath(accountName);
        on_Login_clicked();
    }
}
