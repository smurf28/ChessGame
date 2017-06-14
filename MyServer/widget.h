#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <mytcpserver.h>
#include <QByteArray>
#include "gameboard.h"
#include <QJsonArray>
#include <queue>

namespace Ui {
class Widget;
}
struct point{
    int x;
    int y;
};
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QByteArray generic_json();//生成json串
    QByteArray winMessage_json();
    bool json_tostring(QByteArray byte_array);//解析json串 设置当前x，y，stone变量
    void game_json(QByteArray byte_array);//解析json串

    void go_check();//围棋算法
    bool five_check();//五子棋算法

private slots:
    void updateData(QByteArray byte);
    void connected(int socketId);

private:
    MyTcpServer *tcpserver;
    Ui::Widget *ui;
    bool firstgamer = true;//分配棋子颜色
    bool firstconnect ;
    GoGameBoard Go;
    FiveGameBoard Five;
    QString game;//记录当前游戏是五子棋还是围棋

    size_t x;//当前坐标
    size_t y;
    QString current_stone;//当前棋子的颜色
    StoneType stone;

    QJsonArray jsonArray;//记录remove的点
    int count;//计算remove了多少个点
    bool vis[20][20];//围棋算法访问标记
    std::queue<point> q;

    bool go_bfs(int x,int y,StoneType current,std::queue<point> &save);//围棋广搜算法
signals:
    void sendData(QByteArray byte);
    void disConnect();
};

#endif // WIDGET_H
