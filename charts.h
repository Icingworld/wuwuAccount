#ifndef CHARTS_H
#define CHARTS_H
#include <QChartView>
#include <QChart>
#include <QPieSeries>
#include <QSplineSeries> 
#include <QString>
#include <QList>
#include <QDebug>
QT_CHARTS_USE_NAMESPACE

enum {PIE};

struct Data
{
    int length{0};
    QList <QString> name{};
    QList <double> amount{};
};

class charts
{
public:
    charts();
    ~charts();
    QChartView * createPie(const Data & data, const QString & title);
};

#endif // CHARTS_H
