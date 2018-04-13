#include "outputmanager.h"

OutputManager::OutputManager()
{
    word = new QAxObject("Word.Application");
    word->setProperty("Visible", true);


}

void OutputManager::open(QString File){

    document = word->querySubObject("Documents")->querySubObject("Open(const QString&)", QDir::currentPath().replace("/", "\\") + "\\" + File);;

}

void OutputManager::putNomClient(QString nom){

    document->querySubObject("Bookmarks(const QString &)", "NomClient")->querySubObject("Range()")->dynamicCall("insertBefore(const QString &)", nom);

}

void OutputManager::putWilaya(QString wilaya){

    document->querySubObject("Bookmarks(const QString &)", "Wilaya")->querySubObject("Range()")->dynamicCall("insertBefore(const QString &)", wilaya);

}

void OutputManager::putDate(QDate date){

    document->querySubObject("Bookmarks(const QString&)", "Date")->querySubObject("Range()")->dynamicCall("insertBefore(const QString &)", date.toString("dd/MM/yyyy"));

}

void OutputManager::putNumeroDeFacture(QString numero){

    document->querySubObject("Bookmarks(const QString&)", "NumeroDeFacture")->querySubObject("Range()")->dynamicCall("insertBefore(const QString &)", numero);

}

void OutputManager::putNumeroDeFactureGeneral(QString numero){

    document->querySubObject("Bookmarks(const QString&)", "NumeroDeFactureGeneral")->querySubObject("Range()")->dynamicCall("insertBefore(const QString &)", numero);

}

void OutputManager::addArticleToTable(int id, QString Designation, int Quantity, double PU, double montant){

    QAxObject* tables = document->querySubObject("Range()")->querySubObject("Tables(int)", 1);
    tables->querySubObject("Rows()")->querySubObject("Add()");
    tables->querySubObject("Cell(int, int)", id+1, 1)->querySubObject("Range()")->dynamicCall("SetText(const QString &)", QString::number(id));
    tables->querySubObject("Cell(int, int)", id+1, 2)->querySubObject("Range()")->dynamicCall("SetText(const QString &)", Designation);
    tables->querySubObject("Cell(int, int)", id+1, 3)->querySubObject("Range()")->dynamicCall("SetText(const QString &)", QString::number(Quantity));
    tables->querySubObject("Cell(int, int)", id+1, 4)->querySubObject("Range()")->dynamicCall("SetText(const QString &)", QString::number(PU,'f', 2));
    tables->querySubObject("Cell(int, int)", id+1, 5)->querySubObject("Range()")->dynamicCall("SetText(const QString &)", QString::number(montant,'f', 2));

}

void OutputManager::addTotal(int row, double total){

    QAxObject* tables = document->querySubObject("Range()")->querySubObject("Tables(int)", 1);
    tables->querySubObject("Cell(int, int)", row+2, 4)->querySubObject("Range()")->dynamicCall("SetText(const QString &)", "Total");
    tables->querySubObject("Cell(int, int)", row+2, 5)->querySubObject("Range()")->dynamicCall("SetText(const QString &)", QString::number(total, 'f', 2));

}

void OutputManager::addVR(QString VR){

    document->querySubObject("Bookmarks(const QString&)", "VR")->querySubObject("Range()")->dynamicCall("insertBefore(const QString &)", VR);

}


OutputManager::~OutputManager(){


}

/*QAxObject *document = documents->querySubObject("Open(const QString&)", QDir::currentPath().replace("/", "\\") + "\\" + File);
//QAxObject *nomBookmark = document->querySubObject("Bookmarks(const QString&)", "listDeProduit");
QAxObject* range = document->querySubObject("Range()");
range->dynamicCall("insertBefore(const QString &)", "Test");
QAxObject* tables = range->querySubObject("Tables(int)", 2);
tables->querySubObject("Rows()")->querySubObject("Add()");
tables->querySubObject("Cell(int, int)", 3, 1)->querySubObject("Range()")->dynamicCall("SetText(const QString &)", "Test");
*/
