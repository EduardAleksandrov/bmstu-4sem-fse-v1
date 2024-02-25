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

    ui->radioButton->setChecked(true);
    ui->radioButton_3->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete wh;
    delete ddialog;
}


void MainWindow::on_pushButton_add_clicked()
{
/*  //Пример
    QString name = ui->lineEdit->text();
    ui->lineEdit->setText("");
    if(name == "") ui->listWidget->addItem("empty line");
    qDebug() << "hello to terminal";
    qDebug() << name.toInt();

    QStandardItemModel* model = new QStandardItemModel();// 3 строки, 2 столбца
    model->setRowCount(3);
    model->setColumnCount(2);

    model->setItem(0, 0, new QStandardItem("Tom"));
    model->setItem(0, 1, new QStandardItem("39"));
    model->setItem(1, 0, new QStandardItem("Bob"));
    model->setItem(1, 1, new QStandardItem("43"));
    model->setItem(2, 0, new QStandardItem("Sam"));
    model->setItem(2, 1, new QStandardItem("28"));

    // установка заголовков таблицы
    model->setHeaderData(0, Qt::Horizontal, "Name");
    model->setHeaderData(1, Qt::Horizontal, "Age");

    ui->tableView->setModel(model);
//    ui->tableView->sortByColumn(1);
//    model->clear();
*/

    int err = wh->add(ui);
    if(err != 0) return;
    MainWindow::on_pushButton_show_clicked();
}


void MainWindow::on_pushButton_delete_clicked()
{
    int err = wh->del(ui);
    if(err != 0) return;
    MainWindow::on_pushButton_show_clicked();
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
    ui->listWidget->clear();

    QStandardItemModel* model = new QStandardItemModel();
    model->setRowCount(wh->get_size());
    model->setColumnCount(4);

    // установка заголовков таблицы
    if(ui->checkBox->isChecked())
    {
        model->setHeaderData(0, Qt::Horizontal, "Cell code");
        model->setHeaderData(1, Qt::Horizontal, "Detail code");
        model->setHeaderData(2, Qt::Horizontal, "Quantity per cell");
        model->setHeaderData(3, Qt::Horizontal, "Cell volume");
    } else {
        model->setHeaderData(0, Qt::Horizontal, "Код ячейки");
        model->setHeaderData(1, Qt::Horizontal, "Код детали");
        model->setHeaderData(2, Qt::Horizontal, "Количество в ячейке");
        model->setHeaderData(3, Qt::Horizontal, "Объем ячейки");
    }

    for(int i = 0; i < wh->get_size(); i++)
    {
        model->setItem(i, 0, new QStandardItem(QString::fromStdString(wh[0][i].cell_address)));
        model->setItem(i, 1, new QStandardItem(QString::fromStdString(wh[0][i].item_code)));
        model->setItem(i, 2, new QStandardItem(QString::number(wh[0][i].quantity)));
        model->setItem(i, 3, new QStandardItem(QString::number(wh[0][i].volume)));
    }

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //ширина
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //запрет редактирования

    ui->tableView->setModel(model);
}

//фильтр по коду детали
void MainWindow::on_pushButton_1_clicked()
{
    ui->listWidget->clear();
    ui->tableView->setModel(NULL);

    QString detail_code = ui->lineEdit->text();

    if(detail_code == "")
    {
        ui->listWidget->addItem("Деталь не заполнена");
        ui->listWidget->item(0)->setForeground(Qt::red);
        return;
    }

    std::vector <Cell> result {};
    wh->detail_code_all(result, detail_code);

    QStandardItemModel* model = new QStandardItemModel();
    model->setRowCount(result.size());
    model->setColumnCount(4);

    // установка заголовков таблицы
    if(ui->checkBox->isChecked())
    {
        model->setHeaderData(0, Qt::Horizontal, "Cell code");
        model->setHeaderData(1, Qt::Horizontal, "Detail code");
        model->setHeaderData(2, Qt::Horizontal, "Quantity per cell");
        model->setHeaderData(3, Qt::Horizontal, "Cell volume");
    } else {
        model->setHeaderData(0, Qt::Horizontal, "Код ячейки");
        model->setHeaderData(1, Qt::Horizontal, "Код детали");
        model->setHeaderData(2, Qt::Horizontal, "Количество в ячейке");
        model->setHeaderData(3, Qt::Horizontal, "Объем ячейки");
    }

    for(unsigned int i = 0; i < result.size(); i++)
    {
        model->setItem(i, 0, new QStandardItem(QString::fromStdString(result[i].cell_address)));
        model->setItem(i, 1, new QStandardItem(QString::fromStdString(result[i].item_code)));
        model->setItem(i, 2, new QStandardItem(QString::number(result[i].quantity)));
        model->setItem(i, 3, new QStandardItem(QString::number(result[i].volume)));
    }

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //ширина
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //запрет редактирования

    ui->tableView->setModel(model);
}

//суммарное количество по коду детали
void MainWindow::on_pushButton_2_clicked()
{
    ui->listWidget->clear();
    ui->tableView->setModel(NULL);

    QString detail_code = ui->lineEdit->text();

    if(detail_code == "")
    {
        ui->listWidget->addItem("Деталь не заполнена");
        ui->listWidget->item(0)->setForeground(Qt::red);
        return;
    }

    std::vector <Cell> result {};
    wh->detail_code_sum(result, detail_code);

    QStandardItemModel* model = new QStandardItemModel();
    model->setRowCount(result.size());
    model->setColumnCount(2);

    // установка заголовков таблицы
    if(ui->checkBox->isChecked())
    {
        model->setHeaderData(0, Qt::Horizontal, "Detail code");
        model->setHeaderData(1, Qt::Horizontal, "Total");
    } else {
        model->setHeaderData(0, Qt::Horizontal, "Код детали");
        model->setHeaderData(1, Qt::Horizontal, "Всего");
    }

    for(unsigned int i = 0; i < result.size(); i++)
    {
        model->setItem(i, 0, new QStandardItem(QString::fromStdString(result[i].item_code)));
        model->setItem(i, 1, new QStandardItem(QString::number(result[i].quantity)));
    }

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //ширина
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //запрет редактирования

    ui->tableView->setModel(model);
}

//суммарное количество по каждому коду
void MainWindow::on_pushButton_4_clicked()
{
    ui->listWidget->clear();
    ui->tableView->setModel(NULL);

    std::vector <Cell> result {};
    wh->each_detail_code_sum(result);

    QStandardItemModel* model = new QStandardItemModel();
    model->setRowCount(result.size());
    model->setColumnCount(2);

    // установка заголовков таблицы
    if(ui->checkBox->isChecked())
    {
        model->setHeaderData(0, Qt::Horizontal, "Detail code");
        model->setHeaderData(1, Qt::Horizontal, "Total");
    } else {
        model->setHeaderData(0, Qt::Horizontal, "Код детали");
        model->setHeaderData(1, Qt::Horizontal, "Всего");
    }

    for(unsigned int i = 0; i < result.size(); i++)
    {
        model->setItem(i, 0, new QStandardItem(QString::fromStdString(result[i].item_code)));
        model->setItem(i, 1, new QStandardItem(QString::number(result[i].quantity)));
    }

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //ширина
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //запрет редактирования

    ui->tableView->setModel(model);
}

//сортировка по убыванию адреса ячейки
void MainWindow::on_pushButton_5_clicked()
{
    ui->listWidget->clear();
    bool isChecked = ui->radioButton->isChecked(); //если выбрана, то сортировка по детали, иначе по адресу ячейки
    bool isChecked_desc = ui->radioButton_3->isChecked(); // если выбрана, то по убыванию
    std::vector <Cell*> result {};
    wh->sort_cell_address(result, isChecked, isChecked_desc);

    QStandardItemModel* model = new QStandardItemModel();
    model->setRowCount(result.size());
    model->setColumnCount(4);

    // установка заголовков таблицы
    if(ui->checkBox->isChecked())
    {
        model->setHeaderData(0, Qt::Horizontal, "Cell code");
        model->setHeaderData(1, Qt::Horizontal, "Detail code");
        model->setHeaderData(2, Qt::Horizontal, "Quantity per cell");
        model->setHeaderData(3, Qt::Horizontal, "Cell volume");
    } else {
        model->setHeaderData(0, Qt::Horizontal, "Код ячейки");
        model->setHeaderData(1, Qt::Horizontal, "Код детали");
        model->setHeaderData(2, Qt::Horizontal, "Количество в ячейке");
        model->setHeaderData(3, Qt::Horizontal, "Объем ячейки");
    }

    for(unsigned int i = 0; i < result.size(); i++)
    {
        model->setItem(i, 0, new QStandardItem(QString::fromStdString(result[i]->cell_address)));
        model->setItem(i, 1, new QStandardItem(QString::fromStdString(result[i]->item_code)));
        model->setItem(i, 2, new QStandardItem(QString::number(result[i]->quantity)));
        model->setItem(i, 3, new QStandardItem(QString::number(result[i]->volume)));
    }

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //ширина
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //запрет редактирования

    ui->tableView->setModel(model);
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    ui->listWidget->clear();
    ui->tableView->setModel(NULL);

    if(arg1 != 0)
    {
        ui->pushButton_add->setText("Add");
        ui->pushButton_delete->setText("Delete");
        ui->pushButton_show->setText("Show all");
        ui->label->setText("Detail code");
        ui->label_2->setText("Quantity");
        ui->pushButton_clear->setText("Clear form");
        ui->pushButton_1->setText("Filter by detail code");
        ui->pushButton_2->setText("Total quantity by detail code");
        ui->pushButton_4->setText("Total quantity for each code");
        ui->pushButton_5->setText("Sort");
        ui->radioButton->setText("Details");
        ui->radioButton_2->setText("Cell code");
        ui->radioButton_3->setText("Descending");
        ui->radioButton_4->setText("Ascending");
        ui->pushButton_diagram->setText("Warehouse loading");
    } else {
        ui->pushButton_add->setText("Добавить");
        ui->pushButton_delete->setText("Удалить");
        ui->pushButton_show->setText("Показать все");
        ui->label->setText("Код детали");
        ui->label_2->setText("Количество");
        ui->pushButton_clear->setText("Очистить форму");
        ui->pushButton_1->setText("Фильтр по коду детали");
        ui->pushButton_2->setText("Суммарное количество по коду детали");
        ui->pushButton_4->setText("Суммарное количество по каждому коду");
        ui->pushButton_5->setText("Сортировка");
        ui->radioButton->setText("Детали");
        ui->radioButton_2->setText("Код ячейки");
        ui->radioButton_3->setText("По убыванию");
        ui->radioButton_4->setText("По возрастанию");
        ui->pushButton_diagram->setText("Показать загрузку склада");
    }
}



void MainWindow::on_pushButton_diagram_clicked()
{
    ddialog = new DDialog(this);
    ddialog->show();
    ddialog->show_diagram(wh, ui->checkBox->isChecked());

}

