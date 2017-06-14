#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include<QTcpServer>
#include "mythread.h"
class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);
protected:
    QObject *tcpserver;
    void incomingConnection(int socketId);
signals:
    void  updateData(QByteArray);
     void sendData(QByteArray);
     void Connect(int);
public slots:

};

#endif // MYTCPSERVER_H
