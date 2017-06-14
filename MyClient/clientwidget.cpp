#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QDebug>
#include <QHostAddress>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QMouseEvent>
#include <QLayout>
#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonValue>

clientWidget::clientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::clientWidget)
{
    ui->setupUi(this);
    this->setGeometry(300,100,477,400);

    //初始化参数
    tcpSocket = NULL;
    board = new ChessBoard(this);

    tcpSocket = new QTcpSocket(this);

    //链接
    connect(tcpSocket, &QTcpSocket::connected,
            [=](){
        qDebug()<<"success!";
    });

    //读取数据
    connect(tcpSocket, &QTcpSocket::readyRead,
            [=](){
        //获取对方发送的内容
        QByteArray array = tcpSocket->readAll();
        qDebug()<<array<<"receive";
        if(firstConnect){
            setStone(array);
            firstConnect = false;
        }else{
            if(ui->board_five==board)
                five_json(array);
            else{
                go_json(array);
            }
        }
        //根据当前下棋者在窗口输出信息
        if(board->getCurrentStone()==board->getStone()){
            if(ui->board_five==board){
                ui->five_label->setText("It's your turn!");
            }else{
                ui->go_label->setText("It's your turn!");
            }
        }else{
            if(ui->board_five==board){
                ui->five_label->setText("Waiting...");
            }else{
                ui->go_label->setText("Waiting...");
            }
        }
    });
}

clientWidget::~clientWidget()
{
    delete ui;
}

void clientWidget::on_exit_clicked(){
    this->close();
}
void clientWidget::on_five_clicked(){
    //设置棋盘
    board=ui->board_five;
    board->setcolumn(15);
    board->setRow(15);
    board->setGeometry(0,0,550,550);
    ui->five_exit->setGeometry(550,30,75,75);
    ui->five_label->setGeometry(540,250,100,30);
    ui->stackedWidget->setCurrentIndex(1);
    this->setGeometry(300,30,650,550);

    //链接服务器
    QString ip = "127.0.0.1";
    tcpSocket->connectToHost(QHostAddress(ip),8080);

    //链接成功后 发送当前棋盘类型
    QJsonObject json;
    json.insert("game", QString("Five"));
    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    tcpSocket->write(byte_array);

    connect(board,SIGNAL(clicked(size_t,size_t)), this,SLOT(board_clicked(size_t,size_t)));
}

void clientWidget::on_five_exit_clicked(){
    this->close();
}

void clientWidget::on_go_clicked(){
    //设置棋盘
    board=ui->board_go;
    board->setcolumn(19);
    board->setRow(19);
    board->setGeometry(0,0,690,690);
    ui->go_exit->setGeometry(690,30,75,75);
    ui->go_label->setGeometry(680,250,100,30);
    ui->stackedWidget->setCurrentIndex(2);
    this->setGeometry(300,30,790,690);

    //链接服务器
    QString ip = "127.0.0.1";
    tcpSocket->connectToHost(QHostAddress(ip),8080);

    //链接成功后 发送当前棋盘类型
    QJsonObject json;
    json.insert("game", QString("Go"));
    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    tcpSocket->write(byte_array);

    connect(board,SIGNAL(clicked(size_t,size_t)), this,SLOT(board_clicked(size_t,size_t)));
}

void clientWidget::on_go_exit_clicked(){
    this->close();
}

void clientWidget::board_clicked(size_t x, size_t y){  
    //   QString str=QString("(%1, %2)").arg(x).arg(y);
    QByteArray temp = generic_json(x,y);
    tcpSocket->write(temp);
}

QByteArray clientWidget::generic_json(size_t x, size_t y){
    QJsonObject json;
    json.insert("x", (int)x);
    json.insert("y",(int)y);
    if(board->getStone()==BLACK){
        json.insert("currentstone", QString("BLACK"));
    }else
        json.insert("currentstone", QString("WHITE"));

    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    return byte_array;
}

void clientWidget::setStone(QByteArray byte_array){

    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array, &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject obj = parse_doucment.object();
            if(obj.contains("stone"))
            {
                QJsonValue stone_value = obj.take("stone");
                if(stone_value.isString())
                {
                    QString stone = stone_value.toString();
                    if(stone == "BLACK")
                        board->setStone(BLACK);
                    else
                        board->setStone(WHITE);
                    board->setCurrentStone(BLACK);
                    qDebug()<<stone<<"receive";
                }
            }
        }

    }
}

void clientWidget::five_json(QByteArray byte_array){
    size_t x,y;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array, &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject obj = parse_doucment.object();
            if(obj.contains("y"))
            {
                QJsonValue y_value = obj.take("y");
                if(y_value.isDouble())
                {
                    y = (size_t)y_value.toVariant().toInt();
                }
            }
            if(obj.contains("x"))
            {
                QJsonValue x_value = obj.take("x");
                if(x_value.isDouble())
                {
                    x = (size_t)x_value.toVariant().toInt();
                }
            }
            if(obj.contains("currentstone"))
            {
                QJsonValue stone_value = obj.take("currentstone");
                if(stone_value.isString())
                {
                    QString current_stone = stone_value.toString();
                    if(current_stone == "BLACK"){
                        board->putStone(x,y,BLACK);
                        if(board->getStone()!=BLACK){
                            board->setCurrentStone(board->getStone());
                        }
                    }
                    else{
                        board->putStone(x,y,WHITE);
                        if(board->getStone()!=WHITE){
                            board->setCurrentStone(board->getStone());
                        }
                    }

                }
            }
            if(obj.contains("message"))
            {
                QJsonValue message_value = obj.take("message");
                if(message_value.isString())
                {
                    QString str = message_value.toString();
                    //弹出提示信息
                    int mes=QMessageBox::information(this, "winMessage", str);
                    if(mes==QMessageBox::Ok){
                        this->close();
                    }
                }
            }
        }
    }
}

void clientWidget::go_json(QByteArray byte_array){
    size_t x,y;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array, &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isArray())
        {
            QJsonArray array = parse_doucment.array();
            QJsonValue value = array.at(0);
            if(value.isObject())
            {
                QJsonObject obj = value.toObject();
                if(obj.contains("y"))
                {
                    QJsonValue y_value = obj.take("y");
                    if(y_value.isDouble())
                    {
                        y = (size_t)y_value.toVariant().toInt();
                    }
                }
                if(obj.contains("x"))
                {
                    QJsonValue x_value = obj.take("x");
                    if(x_value.isDouble())
                    {
                        x = (size_t)x_value.toVariant().toInt();
                    }
                }
                if(obj.contains("currentstone"))
                {
                    QJsonValue stone_value = obj.take("currentstone");
                    if(stone_value.isString())
                    {
                        QString current_stone = stone_value.toString();
                        if(current_stone == "BLACK"){
                            board->putStone(x,y,BLACK);
                            if(board->getStone()!=BLACK){
                                board->setCurrentStone(board->getStone());
                            }
                        }
                        else{
                            board->putStone(x,y,WHITE);
                            if(board->getStone()!=WHITE){
                                board->setCurrentStone(board->getStone());
                            }
                        }

                    }
                }
            }

            int size = array.size();
            for(int i=1; i<size;i++)
            {
                value = array.at(i);
                if(value.isObject())
                {
                    QJsonObject obj = value.toObject();
                    size_t rx,ry;
                    if(obj.contains("y"))
                    {
                        QJsonValue y_value = obj.take("y");
                        if(y_value.isDouble())
                        {
                            ry = (size_t)y_value.toVariant().toInt();
                        }
                    }
                    if(obj.contains("x"))
                    {
                        QJsonValue x_value = obj.take("x");
                        if(x_value.isDouble())
                        {
                            rx = (size_t)x_value.toVariant().toInt();
                        }
                    }
                    board->removeStone(rx,ry);
                }
            }
        }
    }

}
