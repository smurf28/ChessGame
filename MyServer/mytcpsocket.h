#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = 0);

signals:
public slots:
    void sendData(QByteArray byte);
};

#endif // MYTCPSOCKET_H
