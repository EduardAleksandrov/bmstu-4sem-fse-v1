#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "warehouse.h"
#include "ddialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_show_clicked();

    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_diagram_clicked();

    void show_diagram();

    void stop_timer();

private:
    Ui::MainWindow *ui;
    Warehouse *wh;
    DDialog *ddialog;
    QTimer* tmr;
};
#endif // MAINWINDOW_H
