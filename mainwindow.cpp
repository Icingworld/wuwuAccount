#include "mainwindow.h"
#include "ui_mainwindow.h"

void addBlockWidget::addButton_clicked()
{
    Database & dbRef = *db;
    add * add_ = new add(dbRef, dbRef.currentDate());
    connect(add_, &add::sendSignal, this, &addBlockWidget::getSignal);
    add_->show();
}

void addBlockWidget::getSignal()
{
    emit sendSignal();
}

/* TODO: 添加/删除种类，修改密码，清空历史 */

MainWindow::MainWindow(Database & dbs, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), db(dbs)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/logo.png"));
    // set date today
    db.setCurrentDate(db.getDate());
    setCalendar();
    showInfo();
    /* set tab */
    scrollContent = new QWidget(this);
    layout = new QVBoxLayout(scrollContent);
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    ui->tabs->addTab(scrollArea, "记录");
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tabs->removeTab(0);
    ui->tabs->removeTab(0);
    /* add record */
    getRecord(db.getDate());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showInfo()
{
    ui->timenow->setText("当前：" + db.getFormattedDate());
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

QDate MainWindow::getDate()
{
    return QDate::currentDate();
}

int MainWindow::getRecord(const QDate & date, bool flag)
{
    if (flag)
    {
        clearBlock();
        // add button
        addBlockWidget * addblockWidget = new addBlockWidget(&db, ui->tab);
        layout->addWidget(addblockWidget);
        connect(addblockWidget, &addBlockWidget::sendSignal, this, &MainWindow::callBack);
    }

    int year = date.year();
    int month = date.month();
    int day = date.day();
    QSqlQuery query = db.query();
    query.prepare("SELECT id, type, mode, amount, note FROM account WHERE year = :year AND month = :month AND day = :day");
    query.bindValue(":year", year);
    query.bindValue(":month", month);
    query.bindValue(":day", day);
    QList <int> record{};
    if (query.exec()) {
        while (query.next()) {
            int ID = query.value(0).toInt();
            record.append(ID);
            QString value1 = query.value(1).toString();
            int value2 = query.value(2).toInt();;
            double value3 = query.value(3).toDouble();
            QString value4 = query.value(4).toString();
            if (value4 == "")
            {
                value4 = "无";
            }
            if (flag)
            {
                addBlock(ID, value1, value2, value3, value4);
            }
        }
    } else {
        qDebug() << "get account record failed:" << query.lastError().text();
    }
    return record.length();
}

void MainWindow::setCalendar()
{
    QDate selectedDate = ui->calendar->selectedDate();
    int year = selectedDate.year();
    int month = selectedDate.month();
    on_calendar_currentPageChanged(year, month);
}

void MainWindow::addBlock(const int id, const QString & type, const int & mode, const double & amount, const QString & note)
{
    QString styleSheet{"QWidget#block { border: 0px solid gray; border-radius: 10px; background-color: rgba(231, 227, 228, 0.7); }"
              "QWidget#block QWidget QLabel { background-color: transparent; color: black; }"};
    if (mode == 0) {
        styleSheet = "QWidget#block { border: 0px solid gray; border-radius: 10px; background-color: rgba(185, 199, 141, 0.5); }"
              "QWidget#block QWidget QLabel { background-color: transparent; color: black; }";
    }
    BlockWidget * blockWidget = new BlockWidget(id, type, amount, note, styleSheet, ui->tab);
    layout->addWidget(blockWidget);
    connect(blockWidget, &BlockWidget::sendDeleteSignal, this, &MainWindow::deleteRecord);
}

void MainWindow::clearBlock()
{
    while (QLayoutItem* item = layout->takeAt(0)) {
        QWidget* widget = item->widget();
        if (widget) {
            delete widget;
        }
        delete item;
    }
}

void MainWindow::on_calendar_clicked(const QDate &date)
{
    clearBlock();
    getRecord(date);
    db.setCurrentDate(date);
}

void MainWindow::on_calendar_currentPageChanged(int year, int month)
{
    QDate firstDate(year, month, 1);
    QDate lastDate = firstDate.addMonths(1).addDays(-1);
    // 遍历当前选择页面的所有日期
    QDate currentDate = firstDate;
    // set text
    QTextCharFormat format;
    format.setForeground(Qt::gray);
    while (currentDate <= lastDate) {
        // 处理当前日期
        // qDebug() << currentDate;
        if (getRecord(currentDate, false) == 0)
        //  if (true)
        {
            // set styleSheet
            ui->calendar->setDateTextFormat(currentDate, format);
        }
        // 前进到下一个日期
        currentDate = currentDate.addDays(1);
    }
}

void MainWindow::callBack()
{
    getRecord(db.currentDate());
}

void MainWindow::deleteRecord(int id)
{
    qDebug() << id;
    QMessageBox::StandardButton result = QMessageBox::question(nullptr, "警告", "确认删除？",
                                                                QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        QSqlQuery query = db.query();
        query.prepare("DELETE FROM account WHERE id=:id");
        query.bindValue(":id", id);
        if (query.exec()) {
            getRecord(db.currentDate());
        } else {
            qDebug() << "Failed to delete data.";
        }
    }
}

void MainWindow::Warning(const QString & string)
{
    QMessageBox messageBox;
    messageBox.setWindowTitle("Warning");
    messageBox.setText(string);
    messageBox.setIcon(QMessageBox::Information);
    messageBox.addButton("确认", QMessageBox::AcceptRole);
    messageBox.exec();
}
