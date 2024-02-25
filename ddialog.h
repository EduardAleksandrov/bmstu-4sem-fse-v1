#ifndef DDIALOG_H
#define DDIALOG_H

#include <QDialog>

#include "warehouse.h"

namespace Ui {
class DDialog;
}

class DDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DDialog(QWidget *parent = nullptr);
    ~DDialog();
    void show_diagram(Warehouse*, bool) const;

private:
    Ui::DDialog *ui;
};

#endif // DDIALOG_H
