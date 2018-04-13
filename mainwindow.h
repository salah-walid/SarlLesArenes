#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSignalMapper>
#include <QSpinBox>
#include <QDate>
#include <QDebug>
#include <QSqlQuery>
#include <QCompleter>
#include <QRegularExpression>
#include "dbmanager.h"
#include "outputmanager.h"
#include "chooseclient.h"
#include "ajouterclient.h"
#include "showf.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void initToolBar();
    void updateCompleter();
    void calculateMontant(int row);
    bool AjouterArticleMB(QString Designation="");
    double calculateTotal();
    void checkStat();
    void genererFactureWord(QString nom, QString wilaya, QAbstractItemModel *model, double total, QString observation, QString numeroDeFacture, QDate date, int numeroDeFactureGeneral);
    void genererBonDeVersement(QString nom, QString wilaya, double versement, QString numeroDeFacture, QDate date, int numeroDeFactureGeneral);
    ~MainWindow();

private slots:
        void changeStackedWidgetPage(int index);
        void on_Recherche_textChanged(const QString & text);
        void on_Ajouter_clicked();
        void on_Supprimer_clicked();
        void on_Modifier_clicked();
        void on_FactureTable_cellDoubleClicked(int row, int column);
        void on_EnleverAricle_clicked();
        void on_choixPrix_currentIndexChanged(int index);
        void finishEditing();
        void on_nettoyer_clicked(bool forced=false);

        void on_Valider_clicked();

        void on_VersementRadio_clicked();

        void on_ReglerRadio_clicked();

        void on_NonReglerRadio_clicked();

        void on_RechercheClient_textChanged(const QString &arg1);

        void on_chooseNomButton_clicked();

        void on_BoutonAjouterClient_clicked();

        void on_SupprimerClient_clicked();

        void on_listDeClient_clicked(const QModelIndex &index);

        void on_GenererBL_clicked();

        void on_FactureDuClient_doubleClicked(const QModelIndex &index);

        void on_SupprimerFacture_clicked();

        void on_AjouteEdit_returnPressed();
        void vider_AjouterEdit();

        void on_GenererBonDeVersement_clicked();

        void on_modifierClient_clicked();

private:
    Ui::MainWindow *ui;
    dbmanager dbman;
    QPushButton *BoutonListeDesProduit;
    QPushButton *BoutonFacture;
    QPushButton *BoutonClient;
    QCompleter completer;
    bool changed;
    ModifOrAdd ma;
    int idModify;
    QString modifyDate;
    int row;
    int column;
};

#endif // MAINWINDOW_H
