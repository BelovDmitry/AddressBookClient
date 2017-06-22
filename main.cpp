#include "tcp_client.h"

#include "mainwindow.h"
#include <QApplication>
#include <QObject>
#include <QByteArray>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    CTcpClient client;

    client.connecting();

    // send changes
    QObject::connect(&w, SIGNAL(sendData(QByteArray)), &client, SLOT(setData(QByteArray)));

    // get all data
    QObject::connect(&client, SIGNAL(sendData(QByteArray)), &w, SLOT(setData(QByteArray)));

    QObject::connect(&client, SIGNAL(quit()), &w, SLOT(deleteLater()), Qt::DirectConnection);

    return a.exec();
}
