#ifndef ADD_H
#define ADD_H

#include <QDialog>
#include <QMessageBox>
#include "database.h"
#include "checkgroup.h"

namespace Ui {
class add;
}

class add : public QDialog
{
    Q_OBJECT

public:
    explicit add(Database db_, QDate date_, QWidget *parent = nullptr);
    ~add();
    void Warning(const QString & string);
    void getTypeList();
    void setType(const int type);
    void addAccount(const int & mode, const QString & type, const double & amount, const int & year, const int & month, const int & day, QString note);

signals:
    void sendSignal();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_income_clicked();
    void on_expenditure_clicked();

private:
    Ui::add *ui;
    Database db;
    QDate date;
    int mode{1};
    Checkgroup check;
    QStringList incomeTypeList;
    QStringList expenditureTypeList;
};

#endif // ADD_H
