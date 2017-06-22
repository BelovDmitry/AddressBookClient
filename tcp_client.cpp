#include "tcp_client.h"
#include <QMessageBox>

CTcpClient::CTcpClient(): QTcpSocket()
{
    connect(this, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    payload.append(QChar(0x01));
}

void CTcpClient::connecting()
{
    qDebug() << "Connecting to host " << QHostAddress::LocalHost << ":" << 80;
    int i = 0; int max_i = 2; bool fl = true;
    while(fl) {
        connectToHost("127.0.0.1", 12345);
        if (waitForConnected(2000))
            break;
        if (i == max_i)
        {
            i = 0;
            if (QMessageBox::warning(Q_NULLPTR,
                                     QObject::trUtf8("Ошибка"),
                                     QObject::trUtf8("Нет соеденения с сервером. Попробывать еще раз?"),
                                     QObject::trUtf8("Да"),
                                     QObject::trUtf8("Нет"),
                                     QString(),
                                     0,
                                     1)){ fl = false; emit quit(); }
        }
        ++i;
    }
}

void CTcpClient::onConnected()
{
    write(payload);
}

void CTcpClient::onDisconnected()
{
    qDebug() << "Disconected";
}

void CTcpClient::onReadyRead()
{
    QByteArray data = readAll();
    if (data.at(0) == QChar(0x01)) {
        data.remove(0,1);
        emit sendData(data);
    }
}

void CTcpClient::onError(QAbstractSocket::SocketError error)
{
    QString errorMessage = "Error: " ;

    switch(error)
    {
    case QAbstractSocket::HostNotFoundError:
        errorMessage.append("The host was not found.");
        break;
    case QAbstractSocket::RemoteHostClosedError:
        errorMessage.append("The remote host is closed.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        errorMessage.append("The connection was refused.");
        break;
    default:
        errorMessage.append("The unknown error.");
    }

    qDebug() << errorMessage;
}

void CTcpClient::setData(QByteArray data)
{
    payload = data;
    connecting();
}
