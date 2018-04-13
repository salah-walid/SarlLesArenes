#include "dbmanager.h"
#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QTimer>
#include <QDebug>
#include <QDate>

dbmanager::dbmanager(){
    qDebug() << QDir::current().absolutePath();
    if(QFile::exists(QDir::currentPath() + "/SarlLesArenes.db")){

        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(QDir::currentPath() + "/SarlLesArenes.db");
        db.open();
    }else{

        QMessageBox::warning(NULL, "Erreur", "Erreur de base de donnee");
        QTimer::singleShot(100, qApp, SLOT(quit()));

    }
}

EditableSqlModel *dbmanager::getListeDesProduit(QString recherche){
    EditableSqlModel *model = new EditableSqlModel();
    QSqlQuery qry = QSqlQuery(db);
    qry.prepare("Select Reference,Designation,PrixGros AS \"Prix Gros\",PrixDetail AS \"Prix Detail\", id From produit WHERE Designation LIKE :Des OR Reference LIKE :Des ORDER BY Designation");
    qry.bindValue(":Des", "%" + recherche + "%");
    qry.exec();
    model->setQuery(qry);
    model->setData(model->index(0,0), QVariant("kihgjk"));
    return model;

}

QSqlQueryModel* dbmanager::getListeDesClient(QString nomClient){

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery qry = QSqlQuery(db);
    qry.prepare("Select NomDuClient FROM Client WHERE NomDuClient LIKE :nom ORDER BY NomDuClient");
    qry.bindValue(":nom", "%" + nomClient + "%");
    qry.exec();
    model->setQuery(qry);
    return model;

}

int dbmanager::getNumeroDeFactureGeneral(){

    QSqlQuery qry = QSqlQuery(db);
    qry.exec("Select key, valueOf FROM information WHERE Key='BLCount'");
    qry.next();
    return qry.value("valueOf").toInt();

}

QSqlQueryModel* dbmanager::getListDesBonDeLivraison(QString nomClient){

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery qry = QSqlQuery(db);
    qry.prepare("Select numeroDuBL AS \"N° BL\" , Etat, Date, Total, id, numeroDuBLGeneral AS \"N° BL general\" FROM BonDeLivraison WHERE nomDuClient= :nom");
    qry.bindValue(":nom", nomClient);
    qry.exec();
    model->setQuery(qry);
    return model;

}

QSqlQuery dbmanager::findClient(QString nomClient){

    QSqlQuery qry = QSqlQuery(db);
    qry.prepare("Select NomDuClient, Wilaya, dernierNumeroDeFacture FROM Client WHERE NomDuClient=:nom");
    qry.bindValue(":nom", nomClient);
    qry.exec();
    return qry;

}

QVector<QVector<QString> > dbmanager::getFacture(int id){

    QSqlQuery qry(db);
    qry.prepare("Select * FROM BonDeLivraison WHERE id = :id");
    qry.bindValue(":id", id);
    qry.exec();
    qry.next();
    QByteArray a = qry.value("ListFacture").toByteArray();
    QVector<QVector<QString> > res = deserialize<QVector<QVector<QString> > >(a);
    return res;

}
//BonDeLivraison (ListFacture

void dbmanager::addProduct(QString Refference, QString Designation, double prixDetail, double prixGros){
    QSqlQuery qry(db);
    qry.prepare("INSERT INTO produit (Reference, Designation, PrixGros, PrixDetail) VALUES( :Ref, :Des , :prixG , :prixD )");
    qry.bindValue(":Ref", Refference);
    qry.bindValue(":Des", Designation);
    qry.bindValue(":prixD", prixDetail);
    qry.bindValue(":prixG", prixGros);
    qry.exec();

}

void dbmanager::addClient(QString nomClient, QString Wilaya){

    QSqlQuery qry(db);
    qry.prepare("INSERT INTO Client(NomDuClient, Wilaya, dernierNumeroDeFacture) VALUES(:nom , :wil , 0)");
    qry.bindValue(":nom", nomClient);
    qry.bindValue(":wil", Wilaya);
    qry.exec();

}

QSqlQueryModel* dbmanager::getCompletionList(){

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery qry = QSqlQuery(db);
    qry.prepare("Select Designation From produit ORDER BY Designation");
    qry.exec();

    model->setQuery(qry);
    return model;

}

void dbmanager::updateProduct(QString Refference, QString Designation, double prixDetail, double prixGros, QString id){

    QSqlQuery qry(db);
    qry.prepare("UPDATE produit SET Reference= :Ref, Designation= :Des, PrixDetail= :prixD, PrixGros= :prixG WHERE id=" + id);
    qry.bindValue(":Ref", Refference);
    qry.bindValue(":Des", Designation);
    qry.bindValue(":prixD", prixDetail);
    qry.bindValue(":prixG", prixGros);
    qry.exec();

}

void dbmanager::updateFacture(QTableView *table, QString nomDuClient, QString Etat, double total,int id){


    QByteArray listeSerialized = serialize(modelToListCostume(table->model()));
    QSqlQuery qry(db);
    //qry.prepare("INSERT INTO BonDeLivraison (ListFacture, nomDuClient, numeroDuBL, Etat, Date, Total, numeroDuBLGeneral) VALUES(:Liste, :nomC, :numeroBL, :Etat, :Dat, :Tot, :numG)");
    qry.prepare("UPDATE BonDeLivraison SET ListFacture= :Liste, nomDuClient= :nomC, Etat= :Etat, Total= :Tot WHERE id= :id");
    qry.bindValue(":Liste", listeSerialized);
    qry.bindValue(":nomC", nomDuClient);
    qry.bindValue(":Etat", Etat);
    qry.bindValue(":Tot", total);
    qry.bindValue(":id", id);
    qry.exec();

}

void dbmanager::updateClient(QString nom, QString Nnom, QString Nwilaya)
{
    QSqlQuery qry(db);
    qry.prepare("UPDATE Client SET NomDuClient= :after WHERE NomDuClient = :before");
    qry.bindValue(":before", nom);
    qry.bindValue(":after", Nnom);
    qry.exec();

    QSqlQuery qry2(db);
    qry2.prepare("UPDATE BonDeLivraison SET nomDuClient= :after WHERE nomDuClient = :before");
    qry2.bindValue(":before", nom);
    qry2.bindValue(":after", Nnom);
    qry2.exec();
}

void dbmanager::updateNumeroDeFacture(int numeroDeFacture, QString nomDuClient){

    QSqlQuery qry(db);
    qry.prepare("UPDATE Client SET dernierNumeroDeFacture= :num WHERE NomDuClient= :nom");
    qry.bindValue(":num", numeroDeFacture);
    qry.bindValue(":nom", nomDuClient);
    qry.exec();

}

void dbmanager::updateNumeroDeFactureGeneral(int numeroDeFactureGeneral){

    QSqlQuery qry(db);
    qry.prepare("UPDATE information SET valueOf= :num WHERE Key='BLCount'");
    qry.bindValue(":num", numeroDeFactureGeneral);
    qry.exec();

}

void dbmanager::deleteProduct(QString id){


    QSqlQuery qry(db);
    qry.exec("DELETE FROM produit WHERE id=" + id);

}

void dbmanager::deleteClient(QString nom){

    QSqlQuery qry(db);
    qry.prepare("DELETE FROM Client WHERE NomDuClient= :nom");
    qry.bindValue(":nom", nom);
    qry.exec();

}

void dbmanager::deleteFacture(int id){

    QSqlQuery qry(db);
    qry.prepare("DELETE FROM BonDeLivraison WHERE id= :id");
    qry.bindValue(":id", id);
    qry.exec();

}

void dbmanager::deleteFacture(QString nomClient){

    QSqlQuery qry(db);
    qry.prepare("DELETE FROM BonDeLivraison WHERE nomDuClient= :nom");
    qry.bindValue(":nom", nomClient);
    qry.exec();

}


QSqlQueryModel *dbmanager::findProduct(int id){

    QSqlQuery qry = QSqlQuery(db);
    qry.prepare("Select Reference,Designation,PrixGros AS \"Prix Gros\",PrixDetail AS \"Prix Detail\", id From produit WHERE id=" + id);
    qry.exec();

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(qry);

    return model;

}

EditableSqlModel *dbmanager::findProduct(QString Designation){

    QSqlQuery qry = QSqlQuery(db);
    qry.prepare("Select Reference,Designation,PrixGros AS \"Prix Gros\",PrixDetail AS \"Prix Detail\" From produit WHERE Designation LIKE :Des");
    qry.bindValue(":Des", "%" + Designation + "%");
    qry.exec();

    EditableSqlModel *model = new EditableSqlModel();
    model->setQuery(qry);

    return model;

}

void dbmanager::addFacture(QTableView *table, QString nomDuClient, int numeroBL, QString Etat, double total, int numeroDuBLGeneral){

    QByteArray listeSerialized = serialize(modelToListCostume(table->model()));
    QSqlQuery qry(db);
    qry.prepare("INSERT INTO BonDeLivraison (ListFacture, nomDuClient, numeroDuBL, Etat, Date, Total, numeroDuBLGeneral) VALUES(:Liste, :nomC, :numeroBL, :Etat, :Dat, :Tot, :numG)");
    qry.bindValue(":Liste", listeSerialized);
    qry.bindValue(":nomC", nomDuClient);
    qry.bindValue(":numeroBL", numeroBL);
    qry.bindValue(":Etat", Etat);
    qry.bindValue(":Dat", QDate::currentDate().toString("dd/MM/yyyy"));
    qry.bindValue(":Tot", total);
    qry.bindValue(":numG", numeroDuBLGeneral);
    qry.exec();

}

dbmanager::~dbmanager(){

    db.close();

}
