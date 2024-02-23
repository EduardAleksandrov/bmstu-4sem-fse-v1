#include "warehouse.h"

Warehouse::Warehouse(QObject* parent): QObject(parent)
{
    Cell i1 {"if500","detail_1",20,100};
    Cell i2 {"if501","detail_2",30,100};
    Cell i3 {"if504","detail_3",50,100};
    Cell i4 {"if508","detail_4",10,100};

    Cell* p[] {&i1, &i2, &i3, &i4};

    for(unsigned int i = 0; i < 20; i++)
    {
        std::string address = "if" + std::to_string(500+i);
        cells.push_back({address, "", 0, 100});
    }

    for(unsigned int i = 0; i < sizeof(p)/sizeof(*p); i++)
    {
        for(unsigned int j = 0; j < 20; j++)
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
