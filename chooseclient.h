#ifndef CHOOSECLIENT_H
#define CHOOSECLIENT_H

#include <QDialog>
#include "dbmanager.h"

namespace Ui {
class chooseClient;
}

class chooseClient : public QDialog
{
    Q_OBJECT

public:
    explicit chooseClient(QWidget *parent, dbmanager *db);
    QString getClientName();
    ~chooseClient();

private slots:
    void on_Recherche_textChanged(const QString &arg1);

private:
    Ui::chooseClient *ui;
    dbmanager *dbman;
};

#endif // CHOOSECLIENT_H
