#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVBoxLayout>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class BlockWidget : public QWidget
{
public:
    BlockWidget(const QString& text1, const QString& text2, QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QVBoxLayout * layout = new QVBoxLayout(this);

        QWidget * buttons = new QWidget(this);
        layout->addWidget(buttons);

        QWidget * types = new QWidget(this);
        layout->addWidget(types);

        QWidget * amounts = new QWidget(this);
        layout->addWidget(amounts);

        QWidget * notes = new QWidget(this);
        layout->addWidget(notes);

        setLayout(layout);
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Database & dbs, QWidget *parent = nullptr);
    ~MainWindow();
    void showInfo();
    void getRecord(const QDate & date);
    // void setCalendar();

private:
    Ui::MainWindow *ui;
    Database db;
};
#endif // MAINWINDOW_H
