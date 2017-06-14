#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "mytcpsocket.h"
#include <QObject>
#include <QThread>
class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent ,int socketId);
private:
    QObject *myparent;
    MyTcpSocket *tcpsocket;
    int socketId;
    void run();
signals:
    void updateData(QByteArray byte);
public slots:
    void readData();
};

#endif // MYTHREAD_H
