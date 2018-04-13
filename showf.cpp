#include "showf.h"
#include "ui_showf.h"
#include <QDebug>
#include <Qt>

showF::showF(QWidget *parent, QVector<QVector<QString> > list) :
    QDialog(parent),
    ui(new Ui::showF)
{
    ui->setupUi(this);
    ui->F->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    for(int i=0; i < list.size(); i++){
        ui->F->insertRow(i);
        ui->F->model()->setData(ui->F->model()->index(i, 0), list.at(i).at(0));
        ui->F->model()->setData(ui->F->model()->index(i, 1), list.at(i).at(1));
        ui->F->model()->setData(ui->F->model()->index(i, 2), list.at(i).at(2));
        ui->F->model()->setData(ui->F->model()->index(i, 3), list.at(i).at(3));
        ui->F->model()->setData(ui->F->model()->index(i, 4), list.at(i).at(4));

    }
}

showF::~showF()
{
    delete ui;
}

QAbstractItemModel* showF::getModel(){
    
    return ui->F->model();
    
}

void showF::on_pushButton_clicked()
{
    ma = add;
    accept();
}

void showF::on_pushButton_2_clicked()
{
    ma = modify;
    accept();
}

ModifOrAdd showF::getMa() const
{
    return ma;
}
