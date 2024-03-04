#include "ddialog.h"
#include "ui_ddialog.h"

#include <QDebug>

using namespace QtCharts;

DDialog::DDialog(QWidget *parent, bool isEng, Warehouse* wh) :
    QDialog(parent),
    ui(new Ui::DDialog)
{
    ui->setupUi(this);

    this->wh = wh;

    if(isEng)
    {
        isEngName = "Volume";
    } else {
        isEngName = "Объем";
    }
    centralFedDistr = new QBarSet(isEngName);

    for(int i = 0; i < wh->get_size(); i++)
    {
        *centralFedDistr << wh[0][i].quantity;
    }

    series = new QBarSeries();
    series->append(centralFedDistr);

    sets = series->barSets();
    float currentHue = 0.0;
    for(int i = 0; i < sets.size(); ++i)
    {
        QColor col = QColor::fromHslF(currentHue, 0.7, 0.5);
        currentHue += 0.618033988749895f;
        currentHue = std::fmod(currentHue, 1.0f);
        sets[i]->setColor(col);
    }

    chart = new QChart();
    chart->addSeries(series);
    if(isEng)
    {
        isEngName = "Warehouse loading";
    } else {
        isEngName = "Загрузка склада";
    }
    chart->setTitle(isEngName);
//    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    for(int i = 0; i < wh->get_size(); i++)
    {
        categories << QString::fromStdString(wh[0][i].cell_address);

    }
    axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis();
    axisY->setRange(0,100);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->horizontalLayout->addWidget(chartView);

    //  chartView->setParent(ui->horizontalFrame);

    //    setCentralWidget(chartView);
    //    resize(800, 400);

}

DDialog::~DDialog()
{
    delete ui;
}
//Диаграмма загрузки склада
void DDialog::show_diagram(Warehouse* whh)
{
    qDebug() << "hello" <<whh[0][0].quantity << " " <<whh[0][1].quantity << " "<< whh[0][4].quantity;
//расчет
    this->wh = whh;

    series->remove(centralFedDistr);
    centralFedDistr = new QBarSet(isEngName);
    for(int i = 0; i < wh->get_size(); i++)
    {
        *centralFedDistr << wh[0][i].quantity;
    }
    series->append(centralFedDistr);
//закраска
    sets = series->barSets();
    float currentHue = 0.0;
    for(int i = 0; i < sets.size(); ++i)
    {
        QColor col = QColor::fromHslF(currentHue, 0.7, 0.5);
        currentHue += 0.618033988749895f;
        currentHue = std::fmod(currentHue, 1.0f);
        sets[i]->setColor(col);
    }
//    chart->addSeries(series);

}
