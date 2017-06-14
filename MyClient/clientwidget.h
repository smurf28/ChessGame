#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include "gameboard.h"
#include <QImage>
#include "chessboard.h"
#include <QJsonObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
namespace Ui {
class clientWidget;
}

class clientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit clientWidget(QWidget *parent = 0);
    ~clientWidget();
    QByteArray generic_json(size_t x,size_t y);//生成json串
    void setStone(QByteArray byte_array);//解析json串
    void five_json(QByteArray byte_array);//解析五子棋传来的数据
    void go_json(QByteArray byte_array);//解析围棋传来的数据

private slots:
    void on_exit_clicked();
    void on_go_clicked();
    void on_five_clicked();
    void on_five_exit_clicked();
    void on_go_exit_clicked();
    void board_clicked(size_t,size_t);

private:
    Ui::clientWidget *ui;
    QTcpSocket *tcpSocket;
    ChessBoard *board;
    bool firstConnect=true;

};

#endif // CLIENTWIDGET_H
