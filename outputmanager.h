#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H
#include <QDebug>
#include <ActiveQt/QAxBase>
#include <ActiveQt/QAxObject>
#include <ActiveQt/QAxWidget>
#include <QDate>
#include <QDir>
#include <QString>

class OutputManager
{
    public:
        OutputManager();
        void open(QString File);
        void putNomClient(QString nom);
        void putWilaya(QString wilaya);
        void putDate(QDate date);
        void putNumeroDeFacture(QString numero);
        void putNumeroDeFactureGeneral(QString numero);
        void addArticleToTable(int id, QString Designation, int Quantity, double PU, double montant);
        void addTotal(int row, double total);
        void addVR(QString VR);
        ~OutputManager();
    private:
        QAxObject* word;
        QAxObject* document;
};

#endif // OUTPUTMANAGER_H
