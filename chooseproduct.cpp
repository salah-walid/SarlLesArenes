#include "chooseproduct.h"
#include "ui_chooseproduct.h"

ChooseProduct::ChooseProduct(QSqlQueryModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseProduct)
{
    ui->setupUi(this);
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

bool ChooseProduct::hasSelected(){

    return ui->tableView->currentIndex().row() != -1;

}

QString ChooseProduct::getDesignation(){
    int row = ui->tableView->currentIndex().row();
    return ui->tableView->model()->data(ui->tableView->model()->index(row,1)).toString();

}

QString ChooseProduct::getRefference(){
    int row = ui->tableView->currentIndex().row();
    return ui->tableView->model()->data(ui->tableView->model()->index(row,0)).toString();

}

double ChooseProduct::getPrixGrox(){
    int row = ui->tableView->currentIndex().row();
    return ui->tableView->model()->data(ui->tableView->model()->index(row,2)).toDouble();

}

double ChooseProduct::getPrixDetail(){
    int row = ui->tableView->currentIndex().row();
    return ui->tableView->model()->data(ui->tableView->model()->index(row,3)).toDouble();

}

ChooseProduct::~ChooseProduct()
{
    delete ui;
}
