#include "warehouse.h"

Warehouse::Warehouse(QObject* parent): QObject(parent)
{
    Cell i1 {"if500","detail_1",20,100};
    Cell i2 {"if501","detail_2",30,100};
    Cell i3 {"if504","detail_3",50,100};
    Cell i4 {"if508","detail_4",10,100};

    Cell* p[] {&i1, &i2, &i3, &i4};

    for(unsigned int i = 0; i < cells_number; i++)
    {
        std::string address = "if" + std::to_string(500+i);
        cells.push_back({address, "", 0, 100});
    }

    for(unsigned int i = 0; i < sizeof(p)/sizeof(*p); i++)
    {
        for(unsigned int j = 0; j < cells_number; j++)
        {
            if(cells[j].cell_address == (*p[i]).cell_address)
            {
                cells[j].item_code = (*p[i]).item_code;
                cells[j].quantity = (*p[i]).quantity;
            }
        }
    }


}
int Warehouse::get_size() const
{
    return cells.size();
}

Cell& Warehouse::operator[](int i)
{
    return cells[i];
}

int Warehouse::add(Ui::MainWindow* ui)
{
    ui->listWidget->clear();
    ui->tableView->setModel(NULL);

    QString detail_code = ui->lineEdit->text();
    QString str_quantity = ui->lineEdit_2->text();
    int detail_quantity = str_quantity.toInt();

    if(detail_code == "" || str_quantity == "")
    {
        ui->listWidget->addItem("Деталь или количество не заполнены");
        ui->listWidget->item(0)->setForeground(Qt::red);
        return 1;
    }

    if(detail_quantity == 0)
    {
        ui->listWidget->addItem("Ошибка в количестве");
        ui->listWidget->item(0)->setForeground(Qt::red);
        return 1;
    }

//расчет
    int sum {0}; //количество свободного места для данной детали на складе
    for(unsigned int i = 0; i < cells_number; i++)
        if(cells[i].quantity == 0 || QString::fromStdString(cells[i].item_code) == detail_code)
            sum+= cells[i].volume - cells[i].quantity;

    if(sum < detail_quantity)
    {
        ui->listWidget->addItem("Не достаточно места на складе");
        ui->listWidget->item(0)->setForeground(Qt::red);
        return 1;
    }
    // добавление к деталям имеющимся на складе
    for(unsigned int i = 0; i < cells_number; i++)
    {
        if(QString::fromStdString(cells[i].item_code) == detail_code && cells[i].volume - cells[i].quantity < 100)
        {
            int diff = cells[i].volume - cells[i].quantity;
            if(detail_quantity > diff)
            {
                detail_quantity-=diff;
                cells[i].quantity +=diff;
                continue;
            }
            if(detail_quantity <= diff)
            {
                cells[i].quantity +=detail_quantity;
                detail_quantity=0;

                break;
            }
        }
    }
    // добавление деталей в пустые ячейки
    if(detail_quantity > 0)
    {
        for(unsigned int i = 0; i < cells_number; i++)
        {
            if((cells[i].volume - cells[i].quantity) == 100)
            {
                int diff = cells[i].volume - cells[i].quantity;
                if(detail_quantity > diff)
                {
                    detail_quantity-=diff;
                    cells[i].quantity +=diff;
                    cells[i].item_code = detail_code.toStdString();
                    continue;
                }
                if(detail_quantity <= diff)
                {
                    cells[i].quantity +=detail_quantity;
                    detail_quantity = 0;

                    cells[i].item_code = detail_code.toStdString();
                    break;
                }
            }
        }
    }
    return 0;
}

int Warehouse::del(Ui::MainWindow* ui)
{
    ui->listWidget->clear();
    ui->tableView->setModel(NULL);

    QString detail_code = ui->lineEdit->text();
    QString str_quantity = ui->lineEdit_2->text();
    int detail_quantity = str_quantity.toInt();

    if(detail_code == "" || str_quantity == "")
    {
        ui->listWidget->addItem("Деталь или количество не заполнены");
        ui->listWidget->item(0)->setForeground(Qt::red);
        return 1;
    }

    if(detail_quantity == 0)
    {
        ui->listWidget->addItem("Ошибка в количестве");
        ui->listWidget->item(0)->setForeground(Qt::red);
        return 1;
    }
//расчет
    int sum {0}; //количество штук для данной детали на складе
    for(unsigned int i = 0; i < cells_number; i++)
        if(QString::fromStdString(cells[i].item_code) == detail_code)
            sum+= cells[i].quantity;

    if(sum < detail_quantity && sum != 0)
    {
        ui->listWidget->addItem("Не достаточно деталей на складе");
        ui->listWidget->item(0)->setForeground(Qt::red);
        return 1;
    } else if(sum == 0)
    {
        ui->listWidget->addItem("Детали нет на складе");
        ui->listWidget->item(0)->setForeground(Qt::red);
        return 1;
    }


    // удаление деталей имеющихся на складе
    for(unsigned int i = 0; i < cells_number; i++)
    {
        if(QString::fromStdString(cells[i].item_code) == detail_code)
        {
            int diff = cells[i].quantity;
            if(detail_quantity > diff)
            {
                cells[i].quantity = 0;
                detail_quantity-=diff;
                cells[i].item_code.clear();
                continue;
            }
            if(detail_quantity <= diff)
            {
                if(detail_quantity == diff) cells[i].item_code.clear();
                cells[i].quantity -= detail_quantity;
                detail_quantity=0;

                break;
            }
        }
    }
    return 0;
}
