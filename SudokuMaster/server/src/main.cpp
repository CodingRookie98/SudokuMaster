//
// Created by hui on 2023/6/26.
//

#include <QCoreApplication>
#include "tcpServer.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    tcpServer server(&a);

    return a.exec();
}
