#ifndef SHOWF_H
#define SHOWF_H

#include <QDialog>
#include <QVector>
#include <QAbstractItemModel>

enum ModifOrAdd{modify, add};

namespace Ui {
class showF;
}

class showF : public QDialog
{
    Q_OBJECT

public:
    explicit showF(QWidget *parent, QVector<QVector<QString> > list);
    QAbstractItemModel* getModel();
    ~showF();

    ModifOrAdd getMa() const;

private slots:
    void on_pushButton_clicked();
    
    void on_pushButton_2_clicked();

private:
    Ui::showF *ui;
    ModifOrAdd ma;
};

#endif // SHOWF_H
