#include "mytcpserver.h"
#include "mythread.h"
#include <QDebug>
MyTcpServer::MyTcpServer(QObject *parent) : QTcpServer(parent),tcpserver(parent)
{
    int port=8080;
    this->listen(QHostAddress::Any,port);
}

void MyTcpServer::incomingConnection(int socketId)
{

    MyThread *th1=new MyThread(tcpserver,socketId);
    th1->start();
    emit this->Connect(socketId);
}
