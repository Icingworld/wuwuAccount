#include "charts.h"

charts::charts()
{

}

charts::~charts()
{

}

QChartView * charts::createPie(const Data & data, const QString & title)
{
    // create a pie series
    QPieSeries * series = new QPieSeries();
    for (int i=0;i<data.length;i++) {
        qDebug() << "show:" << data.name[i] << data.amount[i];
        series->append(data.name[i], data.amount[i]);
    }
    series->setLabelsVisible(true);
    series->setUseOpenGL(true);

    // create pie view
    QChartView *chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);  // anti-aliasing

    // create a pie
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(title);

    // set view
    chartView->setChart(chart);
    return chartView;
}
