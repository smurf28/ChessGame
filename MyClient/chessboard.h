#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QObject>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QImage>
#include "gameboard.h"

class ChessBoard : public QLabel
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent);
    void setStone(StoneType stone);//设置棋手的棋子颜色
    StoneType getStone();//获取

    void setRow(size_t row);
    void setcolumn(size_t column);

    void putStone(size_t x,size_t y,StoneType stone);//在x,y坐标放棋子
    void removeStone(size_t x,size_t y);//拿掉没有气的棋子

    void setCurrentStone(StoneType stone);//设置当前下棋人的棋子
    StoneType getCurrentStone();//获取当前下棋人的棋子
signals:
    void clicked(size_t, size_t);

public slots:
protected:
    void mouseReleaseEvent(QMouseEvent *ev);
    ~ChessBoard(){}
    void paintEvent(QPaintEvent *);
private:
    size_t row;
    size_t column;

    static const size_t padding = 30;
    static const size_t interval = 35;

    StoneType stone;
    StoneType currentstone;//记录当前下的棋子颜色，控制当前用户是否下棋
    GoGameBoard *Go;
    FiveGameBoard *Five;
};

#endif // CHESSBOARD_H
