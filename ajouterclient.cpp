#include "ajouterclient.h"
#include "ui_ajouterclient.h"
#include <QMessageBox>

AjouterClient::AjouterClient(QWidget *parent, dbmanager *manager, mode m, QString nom, QString wilaya) :
    QDialog(parent),
    ui(new Ui::AjouterClient),
    dbman(manager)
{
    ui->setupUi(this);

    if(m == modifier){

        this->setWindowTitle("Modification");
        ui->NomClient->setText(nom);
        ui->Wilaya->setText(wilaya);

    }
}

void AjouterClient::accept(){
    if(ui->NomClient->text().isEmpty() || ui->Wilaya->text().isEmpty()){

        QMessageBox::warning(this, "donnée vide", "Veuillez remplire toute les information");
        return;

    }

    QSqlQuery qry = dbman->findClient(ui->NomClient->text());
    if(qry.next()){
        QMessageBox::warning(this, "Exist deja", "ce nom exist deja");
        return;
    }
    setResult(QDialog::Accepted);
    emit accepted();
    emit finished(result());
    hide();

}

QString AjouterClient::getNomClient(){

    return ui->NomClient->text();

}

QString AjouterClient::getWilaya(){

    return ui->Wilaya->text();

}

AjouterClient::~AjouterClient()
{
    delete ui;
}
