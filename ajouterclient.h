#ifndef AJOUTERCLIENT_H
#define AJOUTERCLIENT_H

#include <QDialog>
#include "dbmanager.h"

enum mode{ajout, modifier};

namespace Ui {
class AjouterClient;
}

class AjouterClient : public QDialog
{
    Q_OBJECT

public:
    explicit AjouterClient(QWidget *parent, dbmanager *manager, mode m=ajout, QString nom="", QString wilaya="");
    QString getNomClient();
    QString getWilaya();
    ~AjouterClient();

private slots:
    void accept();

private:
    Ui::AjouterClient *ui;
    dbmanager *dbman;
};

#endif // AJOUTERCLIENT_H
