#ifndef DDIALOG_H
#define DDIALOG_H

#include <QDialog>
#include <QtCharts>

#include "warehouse.h"

namespace Ui {
class DDialog;
}

class DDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DDialog(QWidget *parent = nullptr, bool isEng = false, Warehouse* wh = nullptr);
    ~DDialog();
    void show_diagram(Warehouse* wh);

private:
    Ui::DDialog *ui;
//для динамики
    QBarSet *centralFedDistr;
    QBarSeries *series;
    QList<QBarSet *> sets;
    QChart *chart;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
    QChartView *chartView;

    QString isEngName;
    Warehouse* wh;
};

#endif // DDIALOG_H
