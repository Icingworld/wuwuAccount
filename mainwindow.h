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
#include <QMessageBox>
#include <QGridLayout>
#include "database.h"
#include "add.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class BlockWidget : public QWidget
{
    Q_OBJECT
public:
    int ID{0};
    // TODO: add a new widget to enable stylesheet
    BlockWidget(const int id, const QString & type, const double & amount, const QString & note, const QString & styleSheet, QWidget* parent = nullptr)
        : QWidget(parent), ID(id)
    {
        setObjectName("block");
        setStyleSheet(styleSheet);

        QVBoxLayout * layout = new QVBoxLayout(this);

        /* type lineeit */
        QWidget * types = new QWidget(this);
        QHBoxLayout* typesLayout = new QHBoxLayout(types);
        QLabel* Label1 = new QLabel("类型:", this);
        QLabel* typeLabel = new QLabel(type, this);
        typesLayout->addWidget(Label1);
        typesLayout->addWidget(typeLabel);
        types->setLayout(typesLayout);
        layout->addWidget(types);

        /* amounts lineeit */
        QWidget * amounts = new QWidget(this);
        QHBoxLayout* amountsLayout = new QHBoxLayout(amounts);
        QLabel* Label2 = new QLabel("金额:", this);
        QLabel* amountsLabel = new QLabel(QString::number(amount), this);
        amountsLayout->addWidget(Label2);
        amountsLayout->addWidget(amountsLabel);
        amounts->setLayout(amountsLayout);
        layout->addWidget(amounts);

        /* notes lineeit */
        QWidget * notes = new QWidget(this);
        QHBoxLayout* notesLayout = new QHBoxLayout(notes);
        QLabel* Label3 = new QLabel("备注:", this);
        QLabel* notesLabel = new QLabel(note, this);
        notesLayout->addWidget(Label3);
        notesLayout->addWidget(notesLabel);
        notes->setLayout(notesLayout);
        layout->addWidget(notes);

        /* buttons */
        QWidget * buttons = new QWidget(this);
        QHBoxLayout* buttonsLayout = new QHBoxLayout(buttons);
        QPushButton* button1 = new QPushButton("编辑", this);
        QPushButton* button2 = new QPushButton("删除", this);
        buttonsLayout->addWidget(button1);
        buttonsLayout->addWidget(button2);
        buttons->setLayout(buttonsLayout);
        layout->addWidget(buttons);

        setLayout(layout);

        connect(button1, &QPushButton::clicked, this, &BlockWidget::sendSignal1);
        connect(button2, &QPushButton::clicked, this, &BlockWidget::sendSignal2);
    }

signals:
    void sendUpdateSignal();
    void sendDeleteSignal(int id);
private slots:
    void sendSignal1()
    {
        emit sendUpdateSignal();
    }
    void sendSignal2()
    {
        emit sendDeleteSignal(ID);
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
    void addButton_clicked();
    void getSignal();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Database & dbs, QWidget *parent = nullptr);
    ~MainWindow();
    void showInfo();
    QDate getDate();
    int getRecord(const QDate & date, bool flag=true);
    void setCalendar();
    void addBlock(const int id, const QString & type, const int & mode, const double & amount, const QString & note);
    void clearBlock();
    void Warning(const QString & string);

private slots:
    void on_calendar_clicked(const QDate &date);
    void on_calendar_currentPageChanged(int year, int month);
    void callBack();
    void deleteRecord(int id);

private:
    Ui::MainWindow *ui;
    Database db;
    QWidget * scrollContent;
    QVBoxLayout * layout;
};
#endif // MAINWINDOW_H
