#include "mythread.h"

MyThread::MyThread(QObject *parent, int socketId) : QThread(parent),socketId(socketId),myparent(parent)
{

}
void MyThread::run()
{
    tcpsocket=new MyTcpSocket(this);
    tcpsocket->setSocketDescriptor(socketId);

    bool b=connect(tcpsocket, SIGNAL(readyRead()),
            this, SLOT(readData()),Qt::DirectConnection);

    connect(this,SIGNAL(updateData(QByteArray)),myparent,SLOT(updateData(QByteArray)));
    connect(myparent, SIGNAL(sendData(QByteArray)),tcpsocket, SLOT(sendData(QByteArray)));

    exec();
}

void MyThread::readData()
{
   QByteArray buffer=tcpsocket->readAll();
   emit updateData(buffer);
}

