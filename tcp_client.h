#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <QDebug>
#include <QThread>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>

class CTcpClient: public QTcpSocket
{
    Q_OBJECT
public:
    CTcpClient();
    ~CTcpClient() = default;

    void connecting();

signals:
    void sendData(QByteArray);
    void quit();

public slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError);
    void setData(QByteArray);
private:
    QByteArray payload;
};

#endif // TCP_CLIENT_H
