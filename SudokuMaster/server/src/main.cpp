//
// Created by hui on 2023/6/26.
//

#include <QCoreApplication>
#include "tcpServer.h"
#include "QDebug"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDeug() << "start";
    tcpServer server(&a);

    return a.exec();
}
