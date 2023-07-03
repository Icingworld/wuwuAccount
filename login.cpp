#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    // 去掉问号
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    checkUser();
}

login::~login()
{
    delete ui;
}

void login::checkUser()
{
    QDir dataDir("data");
    if (!dataDir.exists()) {
        // 如果不存在，则创建"data"文件夹
        dataDir.mkpath(".");
        qDebug() << "Created 'data' folder.";
    } else {
        // 获取所有子文件夹的名称
        QStringList subDirs = dataDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        qDebug() << "Subdirectories:";
        for (const QString& subDir : subDirs) {
            qDebug() << subDir;
        }
    }
}
