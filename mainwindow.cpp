#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QStandardItemModel>
#include "QStandardItem"
#include <QTableView>
#include <vector>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wh = new Warehouse{this};
}

MainWindow::~MainWindow()
{
    delete ui;
    delete wh;
}


void MainWindow::on_pushButton_add_clicked()
{
//    QString name = ui->lineEdit->text();
//    ui->lineEdit->setText("");
//    if(name == "") ui->listWidget->addItem("empty line");
//    qDebug() << "hello to terminal";
//    qDebug() << name.toInt();

//    QStandardItemModel* model=  new QStandardItemModel();// 3 строки, 2 столбца
//    model->setRowCount(3);
//    model->setColumnCount(2);

//    model->setItem(0, 0, new QStandardItem("Tom"));
//    model->setItem(0, 1, new QStandardItem("39"));
//    model->setItem(1, 0, new QStandardItem("Bob"));
//    model->setItem(1, 1, new QStandardItem("43"));
//    model->setItem(2, 0, new QStandardItem("Sam"));
//    model->setItem(2, 1, new QStandardItem("28"));

//    // установка заголовков таблицы
//    model->setHeaderData(0, Qt::Horizontal, "Name");
//    model->setHeaderData(1, Qt::Horizontal, "Age");

//    ui->tableView->setModel(model);
////    ui->tableView->sortByColumn(1);
////    model->clear();

    ui->listWidget->clear();
    ui->tableView->setModel(NULL);

    QString detail_code = ui->lineEdit->text();
    QString str_quantity = ui->lineEdit_2->text();
    int detail_quantity = str_quantity.toInt();

    if(detail_quantity == 0)
    {
        ui->listWidget->addItem("Ошибка в количестве");
        return;
    }

    if(detail_code == "" || str_quantity == "")
    {
        ui->listWidget->addItem("Деталь или количество не заполнены");
        return;
    }

//расчет
    int sum {0}; //количество свободного места для данной детали на складе
    for(unsigned int i = 0; i < cells_number; i++)
        if(wh[0][i].quantity == 0 || QString::fromStdString(wh[0][i].item_code) == detail_code)
            sum+= wh[0][i].volume - wh[0][i].quantity;

    if(sum < detail_quantity)
    {
        ui->listWidget->addItem("Не достаточно места на складе");
        return;
    }
    // добавление к деталям имеющимся на складе
    for(unsigned int i = 0; i < cells_number; i++)
    {
        if(QString::fromStdString(wh[0][i].item_code) == detail_code && wh[0][i].volume - wh[0][i].quantity < 100)
        {
            int diff = wh[0][i].volume - wh[0][i].quantity;
            if(detail_quantity > diff)
            {
                detail_quantity-=diff;
                wh[0][i].quantity +=diff;
                continue;
            }
            if(detail_quantity <= diff)
            {
                wh[0][i].quantity +=detail_quantity;
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
            if((wh[0][i].volume - wh[0][i].quantity) == 100)
            {
                int diff = wh[0][i].volume - wh[0][i].quantity;
                if(detail_quantity > diff)
                {
                    detail_quantity-=diff;
                    wh[0][i].quantity +=diff;
                    wh[0][i].item_code = detail_code.toStdString();
                    continue;
                }
                if(detail_quantity <= diff)
                {
                    wh[0][i].quantity +=detail_quantity;
                    detail_quantity = 0;

                    wh[0][i].item_code = detail_code.toStdString();
                    break;
                }
            }
        }
    }



}


void MainWindow::on_pushButton_delete_clicked()
{
    ui->listWidget->clear();
    ui->tableView->setModel(NULL);

    QString detail_code = ui->lineEdit->text();
    QString str_quantity = ui->lineEdit_2->text();
    int detail_quantity = str_quantity.toInt();

    if(detail_quantity == 0)
    {
        ui->listWidget->addItem("Ошибка в количестве");
        return;
    }

    if(detail_code == "" || str_quantity == "")
    {
        ui->listWidget->addItem("Деталь или количество не заполнены");
        return;
    }
//расчет
    int sum {0}; //количество штук для данной детали на складе
    for(unsigned int i = 0; i < cells_number; i++)
        if(QString::fromStdString(wh[0][i].item_code) == detail_code)
            sum+= wh[0][i].quantity;

    if(sum < detail_quantity)
    {
        ui->listWidget->addItem("Не достаточно деталей на складе");
        return;
    }

    // удаление деталей имеющихся на складе
    for(unsigned int i = 0; i < cells_number; i++)
    {
        if(QString::fromStdString(wh[0][i].item_code) == detail_code)
        {
            int diff = wh[0][i].quantity;
            if(detail_quantity > diff)
            {
                wh[0][i].quantity = 0;
                detail_quantity-=diff;
                wh[0][i].item_code.clear();
                continue;
            }
            if(detail_quantity <= diff)
            {
                if(detail_quantity == diff) wh[0][i].item_code.clear();
                wh[0][i].quantity -= detail_quantity;
                detail_quantity=0;

                break;
            }
        }
    }
}


void MainWindow::on_pushButton_clear_clicked()
{
    ui->tableView->setModel(NULL);
    ui->listWidget->clear();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}


void MainWindow::on_pushButton_show_clicked()
{
    QStandardItemModel* model=  new QStandardItemModel();
    model->setRowCount(wh->get_size());
    model->setColumnCount(4);

    // установка заголовков таблицы
    model->setHeaderData(0, Qt::Horizontal, "Код ячейки");
    model->setHeaderData(1, Qt::Horizontal, "Код детали");
    model->setHeaderData(2, Qt::Horizontal, "Количество в ячейке");
    model->setHeaderData(3, Qt::Horizontal, "Объем ячейки");

    for(int i = 0; i < wh->get_size(); i++)
    {
        model->setItem(i, 0, new QStandardItem(QString::fromStdString(wh[0][i].cell_address)));
        model->setItem(i, 1, new QStandardItem(QString::fromStdString(wh[0][i].item_code)));
        model->setItem(i, 2, new QStandardItem(QString::number(wh[0][i].quantity)));
        model->setItem(i, 3, new QStandardItem(QString::number(wh[0][i].volume)));
    }

    ui->tableView->setModel(model);
}

