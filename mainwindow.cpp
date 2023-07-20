#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Database & dbs, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), db(dbs)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/logo.png"));
    // set date today
    db.setCurrentDate(db.getDate());
    setCalendar();
    // set tab
    scrollContent = new QWidget(this);
    layout = new QVBoxLayout(scrollContent);
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tabs->addTab(scrollArea, "记录");
    scrollContent2 = new QWidget(this);
    layout2 = new QVBoxLayout(scrollContent2);
    QScrollArea* scrollArea2 = new QScrollArea(this);
    scrollArea2->setWidget(scrollContent2);
    scrollArea2->setWidgetResizable(true);
    scrollArea2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tabs->addTab(scrollArea2, "分析");
    ui->tabs->removeTab(0);
    ui->tabs->removeTab(0);
    // add record
    getRecord(db.getDate());

    /* test */
    setAnalyze(db.getDate());
}

MainWindow::~MainWindow()
{
    delete ui;
}

QDate MainWindow::getDate()
{
    return QDate::currentDate();
}

int MainWindow::getRecord(const QDate & date, bool flag)
{
    if (flag)
    {
        clearBlock(layout);
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
        qDebug() << "get account record failed[1]:" << query.lastError().text();
    }
    return record.length();
}

void MainWindow::setAnalyze(const QDate & date)
{
    // check isMonthly
    clearBlock(layout2);
    Data data{};
    charts pieChart;
    QMap <QString, double> map;
    double income{0.0}, expenditure{0.0};
    QString Income{}, Expenditure{};
    int year = date.year();
    int month = date.month();
    int day = date.day();
    QSqlQuery query = db.query();
    if (!isMonthly)
    {
        query.prepare("SELECT type, mode, amount FROM account WHERE year = :year AND month = :month AND day = :day");
        query.bindValue(":year", year);
        query.bindValue(":month", month);
        query.bindValue(":day", day);
    } else {
        query.prepare("SELECT type, mode, amount FROM account WHERE year = :year AND month = :month");
        query.bindValue(":year", year);
        query.bindValue(":month", month);
    }
    if (query.exec()) {
        while (query.next()) {
            QString value1 = query.value(0).toString();
            int value2 = query.value(1).toInt();
            double value3 = query.value(2).toDouble();
            if (value2 == 0)
            {
                // income
                income += value3;
            } else {
                // expenditure
                expenditure += value3;
                // count expenditure situation
                if (!map.contains(value1))
                {
                    map.insert(value1, value3);
                } else {
                    map[value1] += value3;
                }
            }
        }
    } else {
        qDebug() << "get account record failed[2]:" << query.lastError().text();
    }
    // create piechart
    QMap <QString, double> ordered_map = sortMapByValueDescending(map);
    QMap<QString, double>::const_iterator iter;
    int count{0};
    for (iter = ordered_map.constBegin(); iter != ordered_map.constEnd(); ++iter) {
        QString key = iter.key();
        int value = iter.value();
        if (count < 5)
        {
            count++;
            data.name.append(key);
            data.amount.append(value);
            data.length++;
        } else {
            break;
        }
    }
    if (!isMonthly)
    {
        Income = QString("本日收入：%1元").arg(income);
        Expenditure = QString("本日支出：%1元").arg(expenditure);
    } else {
        Income = QString("本月共收入：%1元").arg(income);
        Expenditure = QString("本月共支出：%1元").arg(expenditure);
    }
    QLabel * incomeLabel = new QLabel(Income, this);
    QLabel * expenditureLabel = new QLabel(Expenditure, this);
    layout2->addWidget(incomeLabel);
    layout2->addWidget(expenditureLabel);
    layout2->addWidget(pieChart.createPie(data, "主要支出饼状图"));
}

// must be static used for std::sort
bool MainWindow::compareValues(const QPair<QString, double>& pair1, const QPair<QString, double>& pair2) {
    return pair1.second > pair2.second; // descending sort
}

QMap<QString, double> MainWindow::sortMapByValueDescending(const QMap<QString, double>& inputMap) {
    QList<QPair<QString, double>> tempList;
    for (auto it = inputMap.begin(); it != inputMap.end(); ++it) {
        tempList.append(qMakePair(it.key(), it.value()));
    }

    std::sort(tempList.begin(), tempList.end(), compareValues);

    QMap<QString, double> sortedMap;
    for (const auto& pair : tempList) {
        sortedMap.insert(pair.first, pair.second);
    }

    return sortedMap;
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
    QString styleSheet{"QWidget#block { border: 0px solid gray; border-radius: 10px; background-color: rgba(231, 227, 228, 0.8); }"
              "QWidget#block QWidget QLabel { background-color: transparent; color: black; }"};
    if (mode == 0) {
        styleSheet = "QWidget#block { border: 0px solid gray; border-radius: 10px; background-color: rgba(185, 199, 141, 0.5); }"
              "QWidget#block QWidget QLabel { background-color: transparent; color: black; }";
    }
    BlockWidget * blockWidget = new BlockWidget(id, type, amount, note, styleSheet, ui->tab);
    layout->addWidget(blockWidget);
    connect(blockWidget, &BlockWidget::sendDeleteSignal, this, &MainWindow::deleteRecord);
    connect(blockWidget, &BlockWidget::sendUpdateSignal, this, &MainWindow::updateRecord);
}

void MainWindow::clearBlock(QVBoxLayout * Layout)
{
    while (QLayoutItem* item = Layout->takeAt(0)) {
        QWidget* widget = item->widget();
        if (widget) {
            delete widget;
        }
        delete item;
    }
}

void MainWindow::on_calendar_clicked(const QDate &date)
{
    if (!isStatistics)
    {
        ui->tabs->setCurrentIndex(0);
    }
    clearBlock(layout);
    getRecord(date);
    db.setCurrentDate(date);
    setAnalyze(date);
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

void MainWindow::deleteRecord(const int id)
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
            qDebug() << "Failed to delete data." << query.lastError().text();
        }
    }
}

void MainWindow::updateRecord(const int id, const double amount, const QString & note)
{
    QSqlQuery query = db.query();
    query.prepare("UPDATE account SET amount = :amount, note = :note WHERE id = :id");
    query.bindValue(":amount", amount);
    query.bindValue(":note", note);
    query.bindValue(":id", id);
    if (query.exec()) {
        getRecord(db.currentDate());
    } else {
        qDebug() << "Failed to update data." << query.lastError().text();
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

void MainWindow::on_statistics_clicked()
{
    isMonthly = true;
    isStatistics = true;
    ui->tabs->setTabEnabled(0, false);
    ui->tabs->setCurrentIndex(1);
    setAnalyze(db.currentDate());
}

void MainWindow::on_edit_clicked()
{
    isMonthly = false;
    isStatistics = false;
    ui->tabs->setTabEnabled(0, true);
    on_calendar_clicked(db.currentDate());
}

void MainWindow::on_tabs_currentChanged(int index)
{
    if (index == 1)
    {        
        setAnalyze(db.currentDate());
    }
}

