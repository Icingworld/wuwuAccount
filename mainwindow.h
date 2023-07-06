#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Database & dbs, QWidget *parent = nullptr);
    ~MainWindow();
    void showInfo();


private:
    Ui::MainWindow *ui;
    Database db;
};
#endif // MAINWINDOW_H
