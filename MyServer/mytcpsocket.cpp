#include "mytcpsocket.h"

MyTcpSocket::MyTcpSocket(QObject *parent) : QTcpSocket(parent)
{

}
void MyTcpSocket::sendData(QByteArray byte)
{
    this->write(byte);
}
