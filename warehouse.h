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

    void detail_code_all(std::vector<Cell>& result, QString detail_code);

    void detail_code_sum(std::vector<Cell>& result, QString detail_code);

    void each_detail_code_sum(std::vector<Cell>& result);

    void sort_cell_address(std::vector<Cell*>& result, bool isChecked, bool isChecked_desc);
public slots:

private:
    std::vector <Cell> cells;

};

#endif // WAREHOUSE_H
