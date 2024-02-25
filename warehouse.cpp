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

        if(ui->checkBox->isChecked())
        {
            ui->listWidget->addItem("Detail or quantity not filled in");
        } else {
            ui->listWidget->addItem("Деталь или количество не заполнены");
        }
        ui->listWidget->item(0)->setForeground(Qt::red);
        return 1;
    }

    if(detail_quantity == 0)
    {
        if(ui->checkBox->isChecked())
        {
            ui->listWidget->addItem("Error in quantity");
        } else {
            ui->listWidget->addItem("Ошибка в количестве");
        }

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
        if(ui->checkBox->isChecked())
        {
            ui->listWidget->addItem("Not enough space in the warehouse");
        } else {
            ui->listWidget->addItem("Не достаточно места на складе");
        }

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
                detail_quantity -= diff;
                cells[i].quantity += diff;
                continue;
            }
            if(detail_quantity <= diff)
            {
                cells[i].quantity += detail_quantity;
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
                    detail_quantity -= diff;
                    cells[i].quantity += diff;
                    cells[i].item_code = detail_code.toStdString();
                    continue;
                }
                if(detail_quantity <= diff)
                {
                    cells[i].quantity += detail_quantity;
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
        if(ui->checkBox->isChecked())
        {
            ui->listWidget->addItem("Detail or quantity not filled in");
        } else {
            ui->listWidget->addItem("Деталь или количество не заполнены");
        }
        ui->listWidget->item(0)->setForeground(Qt::red);
        return 1;
    }

    if(detail_quantity == 0)
    {
        if(ui->checkBox->isChecked())
        {
            ui->listWidget->addItem("Error in quantity");
        } else {
            ui->listWidget->addItem("Ошибка в количестве");
        }
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
        if(ui->checkBox->isChecked())
        {
            ui->listWidget->addItem("Not enough details in the warehouse");
        } else {
            ui->listWidget->addItem("Не достаточно деталей на складе");
        }
        ui->listWidget->item(0)->setForeground(Qt::red);
        return 1;
    } else if(sum == 0)
    {
        if(ui->checkBox->isChecked())
        {
            ui->listWidget->addItem("Detail out of warehouse");
        } else {
            ui->listWidget->addItem("Детали нет на складе");
        }

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
                detail_quantity -= diff;
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
//фильтр по коду детали
void Warehouse::detail_code_all(std::vector<Cell>& result, QString detail_code)
{
    for(unsigned int i = 0; i < cells.size(); i++)
    {
        if(cells[i].item_code == detail_code.toStdString())
        {
            result.push_back(cells[i]);
        }
    }
}
//суммарное количество по коду детали
void Warehouse::detail_code_sum(std::vector<Cell>& result, QString detail_code)
{
    int sum {0};
    for(unsigned int i = 0; i < cells.size(); i++)
    {
        if(cells[i].item_code == detail_code.toStdString())
        {
            sum+=cells[i].quantity;
        }
    }
    result.push_back({"",detail_code.toStdString(),sum,0});
}
//суммарное количество по каждому коду детали
void Warehouse::each_detail_code_sum(std::vector<Cell>& result)
{
    std::vector <bool> check (cells.size(), 0);
    unsigned int n = 0;
    while(n < cells.size())
    {
        int sum = 0;
        if(check[n] == 1)
        {
            n++;
            continue;
        }
        for(unsigned int i = n; i < cells.size(); i++)
        {
            if(cells[i].item_code == cells[n].item_code && check[i] == 0)
            {
                sum += cells[i].quantity;
                check[i] = 1;
            }
        }
        if(sum != 0) result.push_back({"",cells[n].item_code,sum,0});
        n++;
    }
}
// сортировка по убыванию адреса ячейки
void Warehouse::sort_cell_address(std::vector<Cell*>& result, bool isChecked, bool isChecked_desc)
{
    for(unsigned int i = 0; i < cells.size(); i++)
    {
        result.push_back(&cells[i]);
    }
    Cell* temp;
    for(unsigned long int i = 0; i < cells.size()-1; i++)
    {
        for(unsigned long int j = i + 1; j < cells.size(); j++)
        {
            if(isChecked_desc == true)
            {
                if(result[i]->cell_address < result[j]->cell_address && isChecked == false)
                {
                    temp = result[i];
                    result[i] = result[j];
                    result[j] = temp;
                }
                if(result[i]->item_code < result[j]->item_code && isChecked == true)
                {
                    temp = result[i];
                    result[i] = result[j];
                    result[j] = temp;
                }
            } else {
                if(result[i]->cell_address > result[j]->cell_address && isChecked == false)
                {
                    temp = result[i];
                    result[i] = result[j];
                    result[j] = temp;
                }
                if(result[i]->item_code > result[j]->item_code && isChecked == true)
                {
                    temp = result[i];
                    result[i] = result[j];
                    result[j] = temp;
                }
            }
        }
    }
}
