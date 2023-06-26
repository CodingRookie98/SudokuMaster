
#include "mainWindow.h"
#include "login.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "SudokuMaster_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    Login login(&w);
    qDebug() << "start";
    qDebug("start");
    std::cout << "start!!";
    w.show();
    login.exec();
    return a.exec();
}