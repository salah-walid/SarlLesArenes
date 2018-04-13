#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QSqlDatabase>
#include <QDir>
#include <QSqlQueryModel>
#include "editablesqlmodel.h"
#include <QSqlQuery>
#include <QByteArray>
#include <QDataStream>
#include <QTableView>
#include <QVector>

template<typename T>
QByteArray serialize(const T& x)
{
    QByteArray ba;
    QDataStream s(&ba, QIODevice::WriteOnly);
    s << x;
    return ba;
}

template<typename T>
T deserialize(QByteArray& ba)
{
    QDataStream s(&ba, QIODevice::ReadOnly);
    T res; s >> res;
    return res;
}




class dbmanager{

public:
    dbmanager();
    ~dbmanager();

    EditableSqlModel *getListeDesProduit(QString recherche="");
    QSqlQueryModel *getListeDesClient(QString nomClient="");
    QSqlQueryModel *getListDesBonDeLivraison(QString nomClient);

    void addProduct(QString Refference, QString Designation, double prixDetail, double prixGros);
    void addClient(QString nomClient, QString Wilaya);
    void addFacture(QTableView *table, QString nomDuClient, int numeroBL, QString Etat, double total, int numeroDuBLGeneral);

    void updateProduct(QString Refference, QString Designation, double prixDetail, double prixGros, QString id);
    void updateNumeroDeFacture(int numeroDeFacture, QString nomDuClient);
    void updateNumeroDeFactureGeneral(int numeroDeFactureGeneral);
    void updateFacture(QTableView *table, QString nomDuClient, QString Etat, double total, int id);
    void updateClient(QString nom, QString Nnom, QString Nwilaya);

    QSqlQueryModel *getCompletionList();

    QSqlQueryModel *findProduct(int id);
    EditableSqlModel *findProduct(QString Designation);
    QVector<QVector<QString> > getFacture(int id);
    int getNumeroDeFactureGeneral();

    QSqlQuery findClient(QString nomClient);

    void deleteProduct(QString id);
    void deleteClient(QString nom);
    void deleteFacture(QString nomClient);
    void deleteFacture(int id);

    QVector<QVector<QString> > modelToListCostume(QAbstractItemModel *model){

        QVector<QVector<QString> > l = QVector<QVector<QString> >();
        for(int i=0; i < model->rowCount();i++){

            QVector<QString> lr= QVector<QString>();
            for(int j=0; j < model->columnCount(); j++){
                lr.push_back(model->data(model->index(i, j)).toString());
            }
            l.push_back(lr);
        }
        return l;
    }

private:
    QSqlDatabase db;
};

#endif // DBMANAGER_H
