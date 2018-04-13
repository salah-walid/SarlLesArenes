#ifndef CHOOSEPRODUCT_H
#define CHOOSEPRODUCT_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class ChooseProduct;
}

class ChooseProduct : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseProduct(QSqlQueryModel *model, QWidget *parent = 0);
    bool hasSelected();
    QString getRefference();
    QString getDesignation();
    double getPrixGrox();
    double getPrixDetail();
    ~ChooseProduct();

private:
    Ui::ChooseProduct *ui;
};

#endif // CHOOSEPRODUCT_H
