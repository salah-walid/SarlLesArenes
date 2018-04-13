#include "chooseclient.h"
#include "ui_chooseclient.h"

chooseClient::chooseClient(QWidget *parent, dbmanager *db) :
    QDialog(parent),
    ui(new Ui::chooseClient),
    dbman(db)
{
    ui->setupUi(this);
    on_Recherche_textChanged("");
    ui->listeClient->setCurrentIndex(ui->listeClient->model()->index(0,0));

}

chooseClient::~chooseClient()
{
    delete ui;
}

void chooseClient::on_Recherche_textChanged(const QString &arg1)
{
    ui->listeClient->setModel(dbman->getListeDesClient(arg1));
}

QString chooseClient::getClientName(){

    return ui->listeClient->currentIndex().data().toString();

}
