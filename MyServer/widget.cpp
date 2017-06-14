#include "widget.h"
#include "ui_widget.h"

#include <QJsonObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    //初始化参数
    game="";
    current_stone="";

    ui->setupUi(this);
   // ui->message->setText("12345");
    setWindowTitle("server");
    tcpserver=new MyTcpServer(this);
    connect(tcpserver,SIGNAL(Connect(int)),this, SLOT(connected(int)));
    connect(tcpserver,SIGNAL(updateData(QByteArray)),this,SLOT(updateData(QByteArray)));//把数据发送服务端页面
}

Widget::~Widget()
{
    delete ui;
}

void Widget::updateData(QByteArray byte)
{
    ui->message->append("receive");
    ui->message->append(byte);
    QByteArray str;
    if(firstconnect){
        this->game_json(byte);
        str = generic_json();
        firstconnect=false;
    }else{
        str = byte;
        bool isjson = this->json_tostring(byte);
        bool win;
        if(isjson){
            if(game=="Five"){
                if(Five.putStone(x,y,stone)){//当前位置是否有棋子
                    win=five_check();
                    if(win)
                        str=winMessage_json();
                }
            }
            if(game=="Go"){
                if(Go.putStone(x,y,stone)){
                    while(!jsonArray.isEmpty()){
                        jsonArray.removeFirst();
                    }
                    //当前的棋子加入jsonArray
                    QJsonObject json;
                    json.insert("x", (int)x);
                    json.insert("y",(int)y);
                    json.insert("currentstone", QString(current_stone));
                    jsonArray.insert(0,json);

                    //检查没有气的棋子
                    go_check();

                    QJsonDocument document;
                    document.setArray(jsonArray);
                    str = document.toJson(QJsonDocument::Compact);

                }
            }
        }
    }
    emit sendData(str);
    ui->message->append("send");
    ui->message->append(str);
}

void Widget::connected(int socketId){

    QString str = QString("socketId:%1 connected!\n").arg(socketId);
    ui->message->append(str);
    firstconnect=true;
}

bool Widget::json_tostring(QByteArray byte_array){
    //解析json串，设置当前x，y，stone变量
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array, &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject obj = parse_doucment.object();
            if(obj.contains("currentstone"))
            {
                QJsonValue stone_value = obj.take("currentstone");
                if(stone_value.isString())
                {
                    current_stone = stone_value.toString();
                    if(current_stone == "BLACK")
                        stone = BLACK;
                    else
                        stone = WHITE;
                }
            }
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
            return true;
        }else{
            return false;
        }
    }
}

void Widget::game_json(QByteArray byte_array){
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array, &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject obj = parse_doucment.object();
            if(obj.contains("game"))
            {
                QJsonValue game_value = obj.take("game");
                if(game_value.isString())
                {
                    game = game_value.toString();
                }
            }
        }
    }
}

QByteArray Widget::generic_json(){

    QJsonObject json;
    if(firstgamer){
        json.insert("stone", QString("BLACK"));
        firstgamer=false;
    }
    else
        json.insert("stone", QString("WHITE"));
    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    return byte_array;
}

QByteArray Widget::winMessage_json(){
    QJsonObject json;
    json.insert("x", (int)x);
    json.insert("y",(int)y);
    json.insert("currentstone", QString(this->current_stone));
    QString temp=this->current_stone+" is win!";
    qDebug()<<temp;
    json.insert("message", QString(temp));
    QJsonDocument document;
    document.setObject(json);
    return document.toJson(QJsonDocument::Compact);
}

bool Widget::go_bfs(int x,int y,StoneType current,std::queue<point> &save){
    //广度优先搜索，如果当前一片区域的棋子没有气，则remove
    int dir[4][2]= {{-1,0},{1,0},{0,-1},{0,1}};//记录四个方向
    point temp;

    //把当前坐标加入队列
    temp.x=x,temp.y=y;
    q.push(temp);
    save.push(temp);
    while(!q.empty())
    {
        //获取队头坐标
        temp=q.front();
        q.pop();
        x=temp.x;
        y=temp.y;
        vis[x][y]=true;
        //对当前坐标的周围进行查看
        for(int i=0; i<4; i++)
        {
            int u,v;
            u=x+dir[i][0];
            v=y+dir[i][1];
            //坐标 u,v 没有棋子 代表有气存在 返回true
            if(this->Go.checkStone(u,v)!=BLACK&&this->Go.checkStone(u,v)!=WHITE)
                return true;
            //否则 将坐标u，v加入队列
            else if(this->Go.checkStone(u,v)==current&&!vis[u][v])
            {
                temp.x=u;
                temp.y=v;
                q.push(temp);
                save.push(temp);
            }
        }//end for
    }
    return false;

}

void Widget::go_check(){
    int dir[4][2]= {{-1,0},{1,0},{0,-1},{0,1}};//记录四个方向
    int u,v;
    count=1;
    //初始化vis
    for(int i=0;i<19;i++)
        for(int j=0;j<19;j++)
            vis[i][j]=false;

    std::queue<point> save;//保存要拿走的棋子

    //1.先看周围的对手的棋子是否有 没有气的区域，有的话则拿走
    for(int i=0; i<4; i++)
    {
        u=x+dir[i][0];
        v=y+dir[i][1];
        if(u<0||u>19||v<0||v>19)
            continue;

        //如果是没有访问过的对手的棋子，进行广搜
        if(Go.checkStone(u,v)!=NONE&&this->Go.checkStone(x,y)!=this->Go.checkStone(u,v)&&!vis[u][v]){

            if(go_bfs(u,v,this->Go.checkStone(u,v),save)){//当前区域有气，则把save和q清空
                while(!save.empty())
                    save.pop();
                while(!q.empty())
                    q.pop();
            }else
            {//当前区域没有气，则将save中的坐标加入json串中并拿走棋子

                while(!save.empty())
                {
                    point temp=save.front();
                    save.pop();
                    QJsonObject json;
                    json.insert("x", temp.x);
                    json.insert("y", temp.y);
                    jsonArray.insert(count++,json);
                    Go.removeStone(temp.x,temp.y);
                }
            }
        }
    }
   // qDebug()<<x<<y<<"after";
    //2.再查看当前棋子的区域有没有气，没有气则移除
    if(go_bfs(x,y,this->Go.checkStone(x,y),save)){
        while(!save.empty())
            save.pop();
        while(!q.empty())
            q.pop();
    }
    while(!save.empty())
    {
        point temp=save.front();
        save.pop();
        QJsonObject json;
        json.insert("x", temp.x);
        json.insert("y", temp.y);
        jsonArray.insert(count++,json);
        Go.removeStone(temp.x,temp.y);
    }
  //  qDebug()<<x<<y<<"end";
}


bool Widget::five_check(){
    for (int i = 0; i < 5; i++)
    {
        //纵向
        if(y - i >= 0 &&
                y + 4 - i <=15 &&
                Five.checkStone(x,y-i)==Five.checkStone(x,y + 1 - i)  &&
                Five.checkStone(x,y-i)==Five.checkStone(x,y + 2 - i) &&
                Five.checkStone(x,y-i)==Five.checkStone(x,y + 3 - i) &&
                Five.checkStone(x,y-i)==Five.checkStone(x,y + 4 - i))
            return true;

        //横行
        if(x - i >= 0 &&
                x + 4 - i <= 15 &&
                Five.checkStone(x - i,y)==Five.checkStone(x + 1 - i,y)  &&
                Five.checkStone(x - i,y)==Five.checkStone(x + 2 - i,y) &&
                Five.checkStone(x - i,y)==Five.checkStone(x + 3 - i,y) &&
                Five.checkStone(x - i,y)==Five.checkStone(x + 4 - i,y))
            return true;

        //左上到右下
        if(x - i >= 0 &&
                y - i >= 0 &&
                x + 4 - i <= 15 &&
                y + 4 - i <= 15 &&
                Five.checkStone(x - i,y - i)==Five.checkStone(x + 1 - i,y + 1 - i)  &&
                Five.checkStone(x - i,y - i)==Five.checkStone(x + 2 - i,y + 2 - i) &&
                Five.checkStone(x - i,y - i)==Five.checkStone(x + 3 - i,y + 3 - i) &&
                Five.checkStone(x - i,y - i)==Five.checkStone(x + 4 - i,y + 4 - i))
            return true;

        //从左下到右上
        if(x + i <= 0xF &&
                y - i >= 0 &&
                x - 4 + i >= 0 &&
                y + 4 - i <= 15 &&
                Five.checkStone(x + i,y - i)==Five.checkStone(x - 1 + i,y + 1 - i)  &&
                Five.checkStone(x + i,y - i)==Five.checkStone(x - 2 + i,y + 2 - i) &&
                Five.checkStone(x + i,y - i)==Five.checkStone(x - 3 + i,y + 3 - i) &&
                Five.checkStone(x + i,y - i)==Five.checkStone(x - 4 + i,y + 4 - i))
            return true;
    }

    return false;

}




