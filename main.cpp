#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QIcon>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load ("qt_" + QLocale::system ().name (), QLibraryInfo::location (QLibraryInfo::TranslationsPath));
    a.installTranslator (&translator);
    a.setWindowIcon(QIcon(QDir::currentPath() + "/favicon.ico"));
    MainWindow w;
    w.setWindowTitle("Sarl Les Arenes");
    w.show();

    return a.exec();
}
