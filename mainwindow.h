#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVBoxLayout>
#include <QLabel>
#include <QDate>
#include <QList>
#include <QPushButton>
#include <QScrollArea>
#include <QTextCharFormat>
#include <QMap>
#include <QMessageBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPair>
#include "database.h"
#include "add.h"
#include "charts.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class BlockWidget : public QWidget
{
    Q_OBJECT
public:
    int ID{0};
    bool isEdit{false};
    QLineEdit * amountsEdit = new QLineEdit("", this);
    QLineEdit * notesEdit = new QLineEdit("", this);
    QPushButton* button1 = new QPushButton("编辑", this);
    QPushButton* button2 = new QPushButton("删除", this);
    
    BlockWidget(const int id, const QString & type, const double & amount, const QString & note, const QString & styleSheet, QWidget* parent = nullptr)
        : QWidget(parent), ID(id)
    {
        setObjectName("block");
        this->setAttribute(Qt::WA_StyledBackground, true);
        setStyleSheet(styleSheet);
        QVBoxLayout * layout = new QVBoxLayout(this);

        /* type lineeit */
        QWidget * types = new QWidget(this);
        QHBoxLayout* typesLayout = new QHBoxLayout(types);
        QLabel* Label1 = new QLabel("类型:", this);
        QLabel* typesLabel = new QLabel(type, this);
        typesLayout->addWidget(Label1);
        typesLayout->addWidget(typesLabel);
        types->setLayout(typesLayout);
        layout->addWidget(types);

        /* amounts lineeit */
        QWidget * amounts = new QWidget(this);
        QHBoxLayout* amountsLayout = new QHBoxLayout(amounts);
        QLabel* Label2 = new QLabel("金额:", this);
        amountsEdit->setText(QString::number(amount));
        amountsLayout->addWidget(Label2);
        amountsLayout->addWidget(amountsEdit);
        amountsEdit->setEnabled(false);
        amounts->setLayout(amountsLayout);
        layout->addWidget(amounts);

        /* notes lineeit */
        QWidget * notes = new QWidget(this);
        QHBoxLayout* notesLayout = new QHBoxLayout(notes);
        QLabel* Label3 = new QLabel("备注:", this);
        notesEdit->setText(note);
        notesLayout->addWidget(Label3);
        notesLayout->addWidget(notesEdit);
        notesEdit->setEnabled(false);
        notes->setLayout(notesLayout);
        layout->addWidget(notes);

        /* buttons */
        QWidget * buttons = new QWidget(this);
        QHBoxLayout* buttonsLayout = new QHBoxLayout(buttons);
        buttonsLayout->addWidget(button1);
        buttonsLayout->addWidget(button2);
        buttons->setLayout(buttonsLayout);
        layout->addWidget(buttons);

        setLayout(layout);

        connect(button1, &QPushButton::clicked, this, &BlockWidget::sendSignal1);
        connect(button2, &QPushButton::clicked, this, &BlockWidget::sendSignal2);
    }

signals:
    void sendUpdateSignal(const int id, const double amount, const QString & note);
    void sendDeleteSignal(const int id);
private slots:
    void sendSignal1()
    {
        if (isEdit)
        {
            emit sendUpdateSignal(ID, amountsEdit->text().toDouble(), notesEdit->text());
        } else {
            isEdit = true;
            amountsEdit->setEnabled(true);
            notesEdit->setEnabled(true);
            button1->setText("确认");
            button2->setText("取消");
        }
    }
    void sendSignal2()
    {
        if (isEdit)
        {
            isEdit = false;
            amountsEdit->setEnabled(false);
            notesEdit->setEnabled(false);
            button1->setText("编辑");
            button2->setText("删除");
        } else {
            emit sendDeleteSignal(ID);
        }
    }
};

class addBlockWidget : public QWidget
{
    Q_OBJECT
public:
    Database * db;
    addBlockWidget(Database * db_, QWidget* parent = nullptr)
        : QWidget(parent), db(db_)
    {
        QVBoxLayout * layout = new QVBoxLayout(this);
        QPushButton * addButton = new QPushButton("+", this);
        layout->addWidget(addButton);
        setLayout(layout);
        QObject::connect(addButton, &QPushButton::clicked, this, &addBlockWidget::addButton_clicked);
    }
signals:
    void sendSignal();
private slots:
    void addButton_clicked()
    {
        Database & dbRef = *db;
        add * add_ = new add(dbRef, dbRef.currentDate());
        connect(add_, &add::sendSignal, this, &addBlockWidget::getSignal);
        add_->show();
    }
    void getSignal()
    {
        emit sendSignal();
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Database & dbs, QWidget *parent = nullptr);
    ~MainWindow();
    QDate getDate();
    int getRecord(const QDate & date, bool flag=true);
    void setAnalyze(const QDate & date);
    static bool compareValues(const QPair<QString, double>& pair1, const QPair<QString, double>& pair2);
    QMap<QString, double> sortMapByValueDescending(const QMap<QString, double>& inputMap);
    void setCalendar();
    void addBlock(const int id, const QString & type, const int & mode, const double & amount, const QString & note);
    void clearBlock(QVBoxLayout * Layout);
    void Warning(const QString & string);

private slots:
    void on_calendar_clicked(const QDate &date);
    void on_calendar_currentPageChanged(int year, int month);
    void callBack();
    void deleteRecord(const int id);
    void updateRecord(const int id, const double amount, const QString & note);
    void on_statistics_clicked();
    void on_edit_clicked();
    void on_tabs_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    Database db;
    bool isMonthly{false}, isStatistics{false};
    QWidget * scrollContent;
    QWidget * scrollContent2;
    QVBoxLayout * layout;
    QVBoxLayout * layout2;
};
#endif // MAINWINDOW_H
