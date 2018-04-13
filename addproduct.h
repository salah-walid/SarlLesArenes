#ifndef ADDPRODUCT_H
#define ADDPRODUCT_H

#include <QDialog>

namespace Ui {
class AddProduct;
}

class AddProduct : public QDialog
{
    Q_OBJECT

public:
    explicit AddProduct(QWidget *parent = 0, QString title="Ajouter un produit", QString Refference="", QString Designation="", double PrixG=0, double PrixD=0);
    ~AddProduct();

    QString getRefference() const;
    QString getDesignation() const;
    double getPrixDetail() const;
    double getPricGros() const;

private slots:
    void accept();

    void on_Designation_textEdited(const QString &arg1);

private:
    Ui::AddProduct *ui;
};

#endif // ADDPRODUCT_H
