#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <QObject>
#include "item.h"
#include <vector>
#include "constants.h"

class Warehouse: public QObject
{
    Q_OBJECT
public:
    Warehouse(QObject* parent = nullptr);
    int get_size() const;
    std::vector<Cell> get_cells() const;
    Cell& operator[](int i);
public slots:
private:
    std::vector <Cell> cells;

};

#endif // WAREHOUSE_H
