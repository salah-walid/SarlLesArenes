#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addproduct.h"
#include "chooseproduct.h"

#include <QMessageBox>
#include <QSqlQueryModel>
#include <QDebug>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    changed = false;
    ma = add;
    idModify = 0;
    ui->setupUi(this);
    on_Recherche_textChanged("");
    on_RechercheClient_textChanged("");

    completer.setFilterMode(Qt::MatchContains);
    completer.setCaseSensitivity(Qt::CaseInsensitive);
    updateCompleter();
    ui->AjouteEdit->setCompleter(&completer);

    ui->dataShow->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->FactureTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    ui->dataShow->setColumnHidden(4,true);
    ui->mainToolBar->setStyleSheet(ui->mainToolBar->styleSheet().append("QToolBar { padding : 10px; } QPushButton{ margin-bottom : 10px; padding : 7px }"));
    this->initToolBar();


}

void MainWindow::updateCompleter(){

    completer.setModel(dbman.getCompletionList());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initToolBar(){
    QSignalMapper* signalMapper = new QSignalMapper (this) ;
    BoutonListeDesProduit = new QPushButton("Gestion De Stock");
    ui->mainToolBar->addWidget(BoutonListeDesProduit);
    connect (BoutonListeDesProduit, SIGNAL(clicked()), signalMapper, SLOT(map())) ;
    signalMapper -> setMapping (BoutonListeDesProduit, 0) ;

    BoutonFacture = new QPushButton("Bon de livraison");
    ui->mainToolBar->addWidget(BoutonFacture);
    connect (BoutonFacture, SIGNAL(clicked()), signalMapper, SLOT(map())) ;
    signalMapper -> setMapping (BoutonFacture, 1) ;

    BoutonClient = new QPushButton("Client");
    ui->mainToolBar->addWidget(BoutonClient);
    connect (BoutonClient, SIGNAL(clicked()), signalMapper, SLOT(map())) ;
    signalMapper -> setMapping (BoutonClient, 2);

    connect (signalMapper, SIGNAL(mapped(int)), this, SLOT(changeStackedWidgetPage(int))) ;

}

void MainWindow::on_EnleverAricle_clicked(){

    int row = ui->FactureTable->currentIndex().row();
    if(row != -1 && QMessageBox::question(this,"Suppression", "Voulez vous vraiment enlever cette article", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){

        ui->FactureTable->removeRow(row);
        calculateTotal();


    }

}

double MainWindow::calculateTotal(){

    double total(0);
    for(int i = 0; i < ui->FactureTable->rowCount(); i++){

        total += ui->FactureTable->model()->data(ui->FactureTable->model()->index(i,4)).toDouble();

    }

    ui->Total->setText("Total : " + QString::number(total, 'f', 2));
    return total;

}

void MainWindow::on_FactureTable_cellDoubleClicked(int row, int column){
    if(column == 3){
        this->row = row;
        this->column = column;
        QSpinBox *edit = new QSpinBox();
        edit->setMaximum(9999999);
        edit->setValue(ui->FactureTable->model()->data(ui->FactureTable->model()->index(row,column)).toInt());
        edit->selectAll();
        connect(edit,SIGNAL(editingFinished()), this, SLOT(finishEditing()));
        QTimer::singleShot(140, edit, SLOT(setFocus()));
        ui->FactureTable->setCellWidget(row, column, edit);
    }else if(column == 2){

        this->row = row;
        this->column = column;
        QDoubleSpinBox *edit = new QDoubleSpinBox();
        edit->setMaximum(9999999.9999999);
        edit->setValue(ui->FactureTable->model()->data(ui->FactureTable->model()->index(row,column)).toDouble());
        edit->selectAll();
        connect(edit,SIGNAL(editingFinished()), this, SLOT(finishEditing()));
        QTimer::singleShot(140, edit, SLOT(setFocus()));
        ui->FactureTable->setCellWidget(row, column, edit);

    }
        //ui->FactureTable->model()->setData(ui->dataShow->model()->index(row,column), QString("osdlsd"));
    //editingFinished()
}

void MainWindow::on_choixPrix_currentIndexChanged(int index){
    if(index == 1)
        ui->FactureTable->setStyleSheet("background-color : rgba(100, 220, 83);");
    else{
        ui->FactureTable->setStyleSheet("background-color : rgba(47, 208, 183);");
    }
    if(ui->FactureTable->rowCount() != 0 && !changed){
        if(QMessageBox::question(this,"Changement", "Si vous changer de prix la liste va etre vider, voulez vous vraiment le faire ?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){

            ui->FactureTable->setRowCount(0);


        }else{
                changed= true;
                if(index == 1)
                    ui->choixPrix->setCurrentIndex(0);
                else{
                    ui->choixPrix->setCurrentIndex(1);
                }

        }

    }else{

        changed = false;

    }

}

void MainWindow::calculateMontant(int row){
    double montant = ui->FactureTable->model()->data(ui->FactureTable->model()->index(row,3)).toDouble() * ui->FactureTable->model()->data(ui->FactureTable->model()->index(row,2)).toDouble();
    ui->FactureTable->model()->setData(ui->FactureTable->model()->index(row,4),
                                       QString::number(montant, 'f', 2));
    calculateTotal();

}

void MainWindow::finishEditing(){
    if(column == 3){
        int value = ((QSpinBox *)ui->FactureTable->cellWidget(row, column))->value();
        ui->FactureTable->removeCellWidget(row, column);
        ui->FactureTable->model()->setData(ui->FactureTable->model()->index(row,column),value);
        calculateMontant(row);
    }else if(column == 2){
        double value = ((QDoubleSpinBox *)ui->FactureTable->cellWidget(row, column))->value();
        ui->FactureTable->removeCellWidget(row, column);
        ui->FactureTable->model()->setData(ui->FactureTable->model()->index(row,column), value);
        calculateMontant(row);

    }

}

void MainWindow::on_Recherche_textChanged(const QString &text){

     ui->dataShow->setModel(dbman.getListeDesProduit(text));

}

bool MainWindow::AjouterArticleMB(QString Designation){

    AddProduct p(this,"Ajouter un produit", "", Designation);
    p.exec();
    if(p.result() == QDialog::Accepted){
        dbman.addProduct(p.getRefference(),p.getDesignation(), p.getPrixDetail(), p.getPricGros());
        on_Recherche_textChanged(ui->Recherche->text());
        QMessageBox::information(this, "Succes", "Article ajouter avec succes");
        updateCompleter();
        return true;
    }else{

        return false;

    }

}

void MainWindow::on_Ajouter_clicked(){

    AjouterArticleMB();

}

void MainWindow::on_Modifier_clicked(){
    int row = ui->dataShow->currentIndex().row();
    if(row != -1){
        AddProduct p(this, QString("Modifier Le produit"),
                     ui->dataShow->model()->data(ui->dataShow->model()->index(row,0)).toString(),
                     ui->dataShow->model()->data(ui->dataShow->model()->index(row,1)).toString(),
                     ui->dataShow->model()->data(ui->dataShow->model()->index(row,2)).toString().toDouble(),
                     ui->dataShow->model()->data(ui->dataShow->model()->index(row,3)).toString().toDouble());
        p.exec();
        if(p.result() == QDialog::Accepted){

            dbman.updateProduct(p.getRefference(),p.getDesignation(), p.getPrixDetail(), p.getPricGros(), ui->dataShow->model()->data(ui->dataShow->model()->index(row,4)).toString());
            on_Recherche_textChanged(ui->Recherche->text());
            updateCompleter();
            QMessageBox::information(this, "Succes", "Article modifier avec succes");

        }
    }
}

void MainWindow::on_Supprimer_clicked(){

    int row = ui->dataShow->currentIndex().row();
    if(row != -1){

        if(QMessageBox::question(this,"Suppression", "Voulez vous vraiment supprimer cette article", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){

            dbman.deleteProduct(ui->dataShow->model()->data(ui->dataShow->model()->index(row,4)).toString());
            on_Recherche_textChanged(ui->Recherche->text());

        }

    }

}

void MainWindow::checkStat(){
    switch(ma){

        case modify:
            ui->TypeLabel->setText("Modification");
        break;
        case add:
            ui->TypeLabel->setText("Ajout");
        break;

    }

}

void MainWindow::changeStackedWidgetPage(int index){

    if(index == 1){

        checkStat();

    }
    ui->stackedWidget->setCurrentIndex(index);

}

void MainWindow::on_nettoyer_clicked(bool forced)
{
    if(forced || (ui->FactureTable->rowCount() != 0 && QMessageBox::question(this,"Annuler", "Voulez vous vraimant annuler", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)){
        ui->FactureTable->setRowCount(0);
        ui->nomClient->clear();
        ui->Wilaya->clear();
        ui->NumeroDeFacture->setValue(0);
        ui->AjouteEdit->setText("");
        ui->NumeroDeFactureGeneral->setValue(0);
        calculateTotal();
        ma = add;
        checkStat();
    }
}

void MainWindow::genererBonDeVersement(QString nom, QString wilaya, double versement, QString numeroDeFacture, QDate date, int numeroDeFactureGeneral){

    try{
        OutputManager wordWriter = OutputManager();
        wordWriter.open("BonDeVersement.docx");
        wordWriter.putNomClient(nom);
        wordWriter.putWilaya(wilaya);
        wordWriter.putDate(ma == add ? date : QDate::fromString(modifyDate, "dd/MM/yyyy"));
        wordWriter.putNumeroDeFacture(numeroDeFacture);
        wordWriter.putNumeroDeFactureGeneral(QString::number(numeroDeFactureGeneral));
        wordWriter.addVR("Versement : " + QString::number(versement, 'f', 2));

    }catch(...){

        QMessageBox::warning(this, "Erreur", "Erreur dans la generation de Bon de livraison");

    }

}

void MainWindow::genererFactureWord(QString nom, QString wilaya, QAbstractItemModel *model, double total, QString observation, QString numeroDeFacture, QDate date, int numeroDeFactureGeneral){
    try{
        OutputManager wordWriter = OutputManager();
        wordWriter.open("BL.docx");
        wordWriter.putNomClient(nom);
        wordWriter.putWilaya(wilaya);
        wordWriter.putDate(ma == add ? date : QDate::fromString(modifyDate, "dd/MM/yyyy"));
        wordWriter.putNumeroDeFacture(numeroDeFacture);
        wordWriter.putNumeroDeFactureGeneral(QString::number(numeroDeFactureGeneral));
        for(int i=0; i < model->rowCount();i++){

            wordWriter.addArticleToTable(i+1, model->data(model->index(i,1)).toString(),
                                         model->data(model->index(i,3)).toInt(),
                                         model->data(model->index(i,2)).toDouble(),
                                         model->data(model->index(i,4)).toDouble());
            //ui->dataShow->model()->data(ui->dataShow->model()->index(row,0)).toString()

        }

        wordWriter.addTotal(model->rowCount(), total);
        wordWriter.addVR(observation);

    }catch(...){

        QMessageBox::warning(this, "Erreur", "Erreur dans la generation de Bon de livraison");

    }


}

void MainWindow::on_Valider_clicked()
{
    if(ui->nomClient->text().isEmpty()){

        QMessageBox::warning(this, "Nom de client", "Veuillez indiquer in nom de client");
        return;

    }else if(ui->FactureTable->rowCount() == 0 && !ui->VersementRadio->isChecked()){

        QMessageBox::warning(this, "Facture vide", "Veuillez ajouter des articles");
        return;

    }else if(ui->Wilaya->text().isEmpty()){

        QMessageBox::warning(this, "Wilaya", "Veuillez indiquer la Wilaya");
        return;

    }
    if(ma == add){
        if(QMessageBox::question(this,"Valider", "Voulez vous vraimant validez la facture", QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
            return;
        if(ui->VersementRadio->isChecked()){

            dbman.addFacture(ui->FactureTable,ui->nomClient->text(),ui->NumeroDeFacture->value(), "Verser = " + ui->Versement->text(), calculateTotal(), ui->NumeroDeFactureGeneral->value());


        }else if(ui->NonReglerRadio->isChecked()){

            dbman.addFacture(ui->FactureTable,ui->nomClient->text(),ui->NumeroDeFacture->value(), "Non Régler", calculateTotal(), ui->NumeroDeFactureGeneral->value());

        }else{

            dbman.addFacture(ui->FactureTable,ui->nomClient->text(),ui->NumeroDeFacture->value(), "Régler", calculateTotal(), ui->NumeroDeFactureGeneral->value());

        }
        dbman.updateNumeroDeFacture(ui->NumeroDeFacture->value(), ui->nomClient->text());
        dbman.updateNumeroDeFactureGeneral(dbman.getNumeroDeFactureGeneral() +1);
        QMessageBox::information(this, "Succes", "Facture validez avec succes");
    }else{

        if(QMessageBox::question(this,"Modifier", "Voulez vous vraiment modifier ce BL", QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
            return;
        if(ui->VersementRadio->isChecked()){

            dbman.updateFacture(ui->FactureTable,ui->nomClient->text(), "Verser = " + ui->Versement->text(), calculateTotal(), idModify);


        }else if(ui->NonReglerRadio->isChecked()){

            dbman.updateFacture(ui->FactureTable,ui->nomClient->text(), "Non Régler", calculateTotal(), idModify);

        }else{

            dbman.updateFacture(ui->FactureTable,ui->nomClient->text(), "Régler", calculateTotal(), idModify);

        }

        QMessageBox::information(this, "Succes", "Facture modifier avec succes");

    }
    on_nettoyer_clicked(true);


}
/*
QStandardItemModel *item = static_cast<QStandardItemModel*>(ui->dataShow->model());
qDebug() << item->data(item->index(1,4)).toString();
*/
void MainWindow::on_VersementRadio_clicked()
{
    ui->Versement->setEnabled(true);
}

void MainWindow::on_ReglerRadio_clicked()
{
    ui->Versement->setEnabled(false);
}

void MainWindow::on_NonReglerRadio_clicked()
{
    ui->Versement->setEnabled(false);
}

void MainWindow::on_RechercheClient_textChanged(const QString &arg1)
{
    ui->listDeClient->setModel(dbman.getListeDesClient(arg1));
}

void MainWindow::on_chooseNomButton_clicked()
{
    if(ma == add || QMessageBox::question(this,"Changement", "Si vous changer le client vous ne serai plus en mode modification, voulez vous vraiment le faire ?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){

        ma = add;
        checkStat();
        chooseClient d(this, &dbman);
        d.exec();
        if(d.result() == QDialog::Accepted){

            QSqlQuery qry = dbman.findClient(d.getClientName());
            qry.next();
            ui->nomClient->setText(qry.value("NomDuClient").toString());
            ui->Wilaya->setText(qry.value("Wilaya").toString().toUpper());
            ui->NumeroDeFacture->setValue(qry.value("dernierNumeroDeFacture").toInt() +1);
            ui->NumeroDeFactureGeneral->setValue(dbman.getNumeroDeFactureGeneral() + 1);

        }else{

            QSqlQuery qry = dbman.findClient(ui->nomClient->text());
            qry.next();
            ui->nomClient->setText(qry.value("NomDuClient").toString());
            ui->Wilaya->setText(qry.value("Wilaya").toString().toUpper());
            ui->NumeroDeFacture->setValue(qry.value("dernierNumeroDeFacture").toInt() +1);
            ui->NumeroDeFactureGeneral->setValue(dbman.getNumeroDeFactureGeneral() + 1);

        }
    }
}

void MainWindow::on_BoutonAjouterClient_clicked()
{
    AjouterClient d(this,&dbman);
    d.exec();
    if(d.result() == QDialog::Accepted){

        dbman.addClient(d.getNomClient(), d.getWilaya());
        on_RechercheClient_textChanged(ui->RechercheClient->text());

    }
}

void MainWindow::on_SupprimerClient_clicked()
{
    if(ui->listDeClient->currentIndex().row() != -1){
         if(QMessageBox::question(this,"Suppression", "Voulez vous vraiment supprimer ce client", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){
            dbman.deleteClient(ui->listDeClient->currentIndex().data().toString());
            dbman.deleteFacture(ui->listDeClient->currentIndex().data().toString());
            on_RechercheClient_textChanged(ui->RechercheClient->text());
            ui->FactureDuClient->setModel(NULL);
         }
    }
}



void MainWindow::on_listDeClient_clicked(const QModelIndex &index)
{
    ui->FactureDuClient->setModel(dbman.getListDesBonDeLivraison(index.data().toString()));
    ui->FactureDuClient->setColumnHidden(4,true);
}

void MainWindow::on_GenererBL_clicked()
{
    if(ui->nomClient->text().isEmpty()){

        QMessageBox::warning(this, "Nom de client", "Veuillez indiquer in nom de client");
        return;

    }else if(ui->FactureTable->rowCount() == 0 && !ui->VersementRadio->isChecked()){

        QMessageBox::warning(this, "Facture vide", "Veuillez ajouter des articles");
        return;

    }else if(ui->Wilaya->text().isEmpty()){

        QMessageBox::warning(this, "Wilaya", "Veuillez indiquer la Wilaya");
        return;

    }

    if(ui->VersementRadio->isChecked()){

        genererFactureWord(ui->nomClient->text(), ui->Wilaya->text(), ui->FactureTable->model(),calculateTotal(), "Verser : " + QString::number(ui->Versement->value(), 'f', 2), ui->NumeroDeFacture->text(), QDate::currentDate(), ui->NumeroDeFactureGeneral->value());

    }else if(ui->NonReglerRadio->isChecked()){

        genererFactureWord(ui->nomClient->text(), ui->Wilaya->text(), ui->FactureTable->model(),calculateTotal(), QString("Non Régler"), ui->NumeroDeFacture->text(), QDate::currentDate(), ui->NumeroDeFactureGeneral->value());

    }else{

        genererFactureWord(ui->nomClient->text(), ui->Wilaya->text(), ui->FactureTable->model(),calculateTotal(), QString("Régler"), ui->NumeroDeFacture->text(), QDate::currentDate(), ui->NumeroDeFactureGeneral->value());

    }
}


void MainWindow::on_FactureDuClient_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    showF f(this, dbman.getFacture(ui->FactureDuClient->model()->data(ui->FactureDuClient->model()->index(index.row(), 4)).toInt()));

    if(f.exec() == QDialog::Accepted){
        ma = f.getMa();
        QSqlQuery client = dbman.findClient(ui->listDeClient->currentIndex().data().toString());
        client.next();
        QString nom = client.value("NomDuClient").toString();
        QString wilaya = client.value("Wilaya").toString();
        QAbstractItemModel* model = f.getModel();
        switch(ma){
            case modify:{

                on_nettoyer_clicked(true);
                ui->nomClient->setText(nom);
                ui->Wilaya->setText(wilaya);
                ui->NumeroDeFacture->setValue(ui->FactureDuClient->model()->data(ui->FactureDuClient->model()->index(row, 0)).toInt());
                ui->NumeroDeFactureGeneral->setValue(ui->FactureDuClient->model()->data(ui->FactureDuClient->model()->index(row, 5)).toInt());
                for(int i=0; i < model->rowCount(); i++){
                    ui->FactureTable->insertRow(i);
                    for(int j=0; j < model->columnCount(); j++){

                        ui->FactureTable->model()->setData(ui->FactureTable->model()->index(i,j), model->data(model->index(i,j)).toString());

                    }

                }
                calculateTotal();
                QRegularExpression re("(?<obs>(Régler|Non Régler|Verser))(?: = (?<vers>(.)*))?");
                QString obs = re.match(ui->FactureDuClient->model()->data(ui->FactureDuClient->model()->index(row, 1)).toString()).captured("obs");
                QString vers = re.match(ui->FactureDuClient->model()->data(ui->FactureDuClient->model()->index(row, 1)).toString()).captured("vers");
                    if(obs == "Régler"){
                        ui->ReglerRadio->setChecked(true);
                        ui->Versement->setEnabled(false);
                    }
                    else if(obs == "Non Régler"){
                        ui->NonReglerRadio->setChecked(true);
                        ui->Versement->setEnabled(false);
                    }
                    else{
                        ui->VersementRadio->setChecked(true);
                        ui->Versement->setValue(vers.replace(",", ".").toDouble());
                        ui->Versement->setEnabled(true);
                    }


                idModify = ui->FactureDuClient->model()->data(ui->FactureDuClient->model()->index(row, 4)).toInt();
                modifyDate = ui->FactureDuClient->model()->data(ui->FactureDuClient->model()->index(row, 2)).toString();
                ma= modify;
                changeStackedWidgetPage(1);
            }break;
            case add:{
                genererFactureWord(nom, wilaya, f.getModel(),
                                   ui->FactureDuClient->model()->data(ui->FactureDuClient->model()->index(row, 3)).toDouble(),
                                    ui->FactureDuClient->model()->data(ui->FactureDuClient->model()->index(row, 1)).toString(),
                                    ui->FactureDuClient->model()->data(ui->FactureDuClient->model()->index(row, 0)).toString(),
                                   QDate::fromString( ui->FactureDuClient->model()->data(ui->FactureDuClient->model()->index(row, 2)).toString(), "dd/MM/yyyy"),
                                   ui->FactureDuClient->model()->data(ui->FactureDuClient->model()->index(row, 5)).toInt());
            }break;

        }
    }
}

void MainWindow::on_SupprimerFacture_clicked()
{
    int row = ui->FactureDuClient->currentIndex().row();
    if(row != -1 && QMessageBox::question(this,"Suppression", "Voulez vous vraiment supprimer ce BL", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){

       dbman.deleteFacture(ui->FactureDuClient->model()->data(ui->FactureDuClient->model()->index(row, 4)).toInt());
       on_listDeClient_clicked(ui->listDeClient->currentIndex());

    }
}

void MainWindow::on_AjouteEdit_returnPressed()
{
    if(!ui->AjouteEdit->text().isEmpty()){
        EditableSqlModel *model = dbman.findProduct(ui->AjouteEdit->text());
        if(model->rowCount() == 0){
            if(QMessageBox::question(this,"Exist pas", "Ce produit n'exist voulez vous aller l'ajouter maintenant ?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
                AjouterArticleMB(ui->AjouteEdit->text());

        }else if(model->rowCount() == 1){
            ui->FactureTable->insertRow(ui->FactureTable->rowCount());
            ui->FactureTable->model()->setData(ui->FactureTable->model()->index(ui->FactureTable->rowCount()-1,0), model->data(model->index(0,0)));
            ui->FactureTable->model()->setData(ui->FactureTable->model()->index(ui->FactureTable->rowCount()-1,1), model->data(model->index(0,1)));
            if(ui->choixPrix->currentIndex() == 0)
                ui->FactureTable->model()->setData(ui->FactureTable->model()->index(ui->FactureTable->rowCount()-1,2), QString::number(model->data(model->index(0,2)).toDouble(), 'f', 2));
            else
                ui->FactureTable->model()->setData(ui->FactureTable->model()->index(ui->FactureTable->rowCount()-1,2), QString::number(model->data(model->index(0,3)).toDouble(), 'f', 2));
            ui->FactureTable->model()->setData(ui->FactureTable->model()->index(ui->FactureTable->rowCount()-1,3), 1);
            calculateMontant(ui->FactureTable->rowCount() - 1);
            ui->FactureTable->scrollToBottom();
            on_FactureTable_cellDoubleClicked(ui->FactureTable->rowCount() - 1, 3);

        }else{

            ChooseProduct p(model, this);
            p.exec();
            if(p.result() == QDialog::Accepted && p.hasSelected()){

                ui->FactureTable->insertRow(ui->FactureTable->rowCount());
                ui->FactureTable->model()->setData(ui->FactureTable->model()->index(ui->FactureTable->rowCount()-1,0), p.getRefference());
                ui->FactureTable->model()->setData(ui->FactureTable->model()->index(ui->FactureTable->rowCount()-1,1), p.getDesignation());
                if(ui->choixPrix->currentIndex() == 0)
                    ui->FactureTable->model()->setData(ui->FactureTable->model()->index(ui->FactureTable->rowCount()-1,2), QString::number(p.getPrixGrox(), 'f', 2));
                else
                    ui->FactureTable->model()->setData(ui->FactureTable->model()->index(ui->FactureTable->rowCount()-1,2), QString::number(p.getPrixDetail(), 'f', 2));
                ui->FactureTable->model()->setData(ui->FactureTable->model()->index(ui->FactureTable->rowCount()-1,3), 1);
                calculateMontant(ui->FactureTable->rowCount() - 1);
                ui->FactureTable->scrollToBottom();
                on_FactureTable_cellDoubleClicked(ui->FactureTable->rowCount() - 1, 3);

            }

        }
        delete model;
    }
    QTimer::singleShot(140, this, SLOT(vider_AjouterEdit()));
}

void MainWindow::vider_AjouterEdit(){

    ui->AjouteEdit->setText("");

}

void MainWindow::on_GenererBonDeVersement_clicked()
{
    if(!ui->VersementRadio->isChecked()){

        QMessageBox::warning(this, "versement", "Veuillez entrer un versement");
        return;

    }else if(ui->nomClient->text().isEmpty()){

        QMessageBox::warning(this, "Nom de client", "Veuillez indiquer in nom de client");
        return;

    }else if(ui->Wilaya->text().isEmpty()){

        QMessageBox::warning(this, "Wilaya", "Veuillez indiquer la Wilaya");
        return;

    }

    genererBonDeVersement(ui->nomClient->text(),
                          ui->Wilaya->text(),
                          ui->Versement->value(),
                          ui->NumeroDeFacture->text(),
                          QDate::currentDate(),
                          ui->NumeroDeFactureGeneral->value());
}


void MainWindow::on_modifierClient_clicked()
{
    int index = ui->listDeClient->currentIndex().row();
    if(index != -1){
        QSqlQuery qry = dbman.findClient(ui->listDeClient->model()->data(ui->listDeClient->model()->index(index, 0)).toString());
        qry.next();
        //qDebug() << qry.value(1);
        AjouterClient p(this, &dbman, modifier, qry.value(0).toString(), qry.value(1).toString());
        p.exec();
        if(p.result() == QDialog::Accepted){

            dbman.updateClient(qry.value(0).toString(), p.getNomClient(), p.getWilaya());
            on_RechercheClient_textChanged(ui->RechercheClient->text());

        }
    }
}
