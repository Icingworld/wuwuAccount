#include "add.h"
#include "ui_add.h"

add::add(Database db_, QDate date_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add), db(db_), date(date_)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/logo.png"));
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    check.add(ui->income);
    check.add(ui->expenditure);
    check.select(1);
    getTypeList();
    setType(1);
}

add::~add()
{
    delete ui;
}

/* get income and expenditure types */
void add::getTypeList()
{
    QSqlQuery query = db.query();
    if (query.exec("SELECT name, mode FROM type")) {
        while (query.next()) {
            if (query.value(1).toString() == "0")
            {
                incomeTypeList.append(query.value(0).toString());
            } else {
                expenditureTypeList.append(query.value(0).toString());
            }
        }
    } else {
        qDebug() << "get type failed:" << query.lastError().text();
    }
    db.open();
}

/* set types in combobox */
void add::setType(const int type)
{
    ui->type->clear();
    if (type == 0)
    {
        // income
        ui->type->addItems(incomeTypeList);
    } else if (type == 1) {
        // expenditure
        ui->type->addItems(expenditureTypeList);
    }
}

/* add a record */
void add::addAccount(const int & mode_, const QString & type, const double & amount, const int & year, const int & month, const int & day, QString note)
{
    db.open();
    QSqlQuery query = db.query();
    query.prepare("INSERT INTO account ([mode], type, amount, year, month, day, note) VALUES (:mode, :type, :amount, :year, :month, :day, :note)");
    query.bindValue(":mode", mode_);
    query.bindValue(":type", type);
    query.bindValue(":amount", amount);
    query.bindValue(":year", year);
    query.bindValue(":month", month);
    query.bindValue(":day", day);
    query.bindValue(":note", note);
    qDebug() << mode_ << type << amount << year << month << day << note;
    query.exec();
}

/* confirm button clicked */
void add::on_buttonBox_accepted()
{
    int year = date.year();
    int month = date.month();
    int day = date.day();
    QString type = ui->type->currentText();
    double amount = ui->amount->text().toDouble();
    QString note = ui->note->text();
    if (ui->amount->text() == "")
    {
        Warning("金额不能为空！");
        return;
    }
    addAccount(mode, type, amount, year, month, day, note);
    emit sendSignal();
}

/* cancel button clicked */
void add::on_buttonBox_rejected()
{
    db.open();
    reject();
}

/* income box clicked */
void add::on_income_clicked()
{
    mode = 0;
    setType(0);
    check.select(0);
}

/* expenditure box clicked */
void add::on_expenditure_clicked()
{
    mode = 1;
    setType(1);
    check.select(1);
}

void add::Warning(const QString & string)
{
    QMessageBox messageBox;
    messageBox.setWindowTitle("Warning");
    messageBox.setText(string);
    messageBox.setIcon(QMessageBox::Information);
    messageBox.addButton("确认", QMessageBox::AcceptRole);
    messageBox.exec();
}
