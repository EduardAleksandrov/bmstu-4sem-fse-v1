#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <QObject>
#include "item.h"
#include <vector>
#include "constants.h"

#include "ui_mainwindow.h"

class Warehouse: public QObject
{
    Q_OBJECT
public:
    Warehouse(QObject* parent = nullptr);

    int get_size() const;

    Cell& operator[](int i);

    int add(Ui::MainWindow* ui);

    int del(Ui::MainWindow* ui);
public slots:

private:
    std::vector <Cell> cells;

};

#endif // WAREHOUSE_H
