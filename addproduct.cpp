#include "addproduct.h"
#include "ui_addproduct.h"
#include <QMessageBox>

AddProduct::AddProduct(QWidget *parent, QString title, QString Refference, QString Designation, double PrixG, double PrixD) :
    QDialog(parent),
    ui(new Ui::AddProduct)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    ui->groupBox->setTitle(title);
    ui->Refference->setText(Refference);
    ui->Designation->setText(Designation);
    ui->PrixGros->setValue(PrixG);
    ui->PrixDetail->setValue(PrixD);
}

void AddProduct::accept(){

    if(ui->Designation->text().isEmpty() || ui->Refference->text().isEmpty()){

        QMessageBox::warning(this, "donnée vide", "Veuillez remplire toute les information");
        return;

    }

    setResult(QDialog::Accepted);
    emit accepted();
    emit finished(result());
    hide();

}

AddProduct::~AddProduct()
{
    delete ui;
}

QString AddProduct::getRefference() const
{
    return ui->Refference->text();
}

QString AddProduct::getDesignation() const
{
    return ui->Designation->text();
}

double AddProduct::getPrixDetail() const
{
    return ui->PrixDetail->value();
}

double AddProduct::getPricGros() const
{
    return ui->PrixGros->value();
}



void AddProduct::on_Designation_textEdited(const QString &arg1)
{
    if(arg1.size() == 1){

        ui->Designation->setText(arg1.toUpper());

    }
}
