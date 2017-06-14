#include "chessboard.h"
#include <QDebug>
ChessBoard::ChessBoard(QWidget *parent) : QLabel(parent)
{
    row=0;
    column=0;
    Go = new  GoGameBoard();
    Five = new FiveGameBoard();
}
void ChessBoard::mouseReleaseEvent(QMouseEvent *ev){
    if(this->currentstone!=this->stone){
       return;
    }
    int GridSize=(height()-padding*2)/(row-1);
    size_t row=(ev->x()-17)/GridSize;
    size_t column=(ev->y()-17)/GridSize;
    if(this->row==19){
        if(Go->putStone(row,column,stone)){
            emit clicked(row, column);
        }
    }else if(this->row==15){
        if(Five->putStone(row,column,stone)){
            emit clicked(row, column);
        }
    }
    update();
    if(currentstone==WHITE)
        currentstone=BLACK;
    else if(currentstone==BLACK)
        currentstone=WHITE;
    //qDebug()<<row<<column;
}

void ChessBoard::paintEvent(QPaintEvent *){
    //画棋盘
    const size_t row_pix = padding + interval * (row -1) +padding;//棋盘宽度
    //qDebug()<<row_pix;
    const size_t column_pix = padding + interval * (column - 1) + padding;//棋盘高

    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(4);
    painter.setPen(pen);
    painter.drawRect(padding,padding,interval * (row -1),interval * (row -1));

    pen.setWidth(1);
    painter.setPen(pen);
    for(size_t i=0;i<row ;i++){
        painter.drawLine(padding, padding+i*interval,column_pix - padding, padding+i*interval);
    }
    for(size_t j=0;j<column ;++j){
        painter.drawLine(padding+j*interval, padding, padding + j*interval, row_pix - padding);
    }

    //画天元
    QBrush brush;
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
        int center=(row -1)/2;
        painter.drawEllipse(QPoint(padding+center*interval,padding+center*interval),4,4);
        painter.drawEllipse(QPoint(padding+3*interval,padding+3*interval),4,4);
        painter.drawEllipse(QPoint(padding+3*interval,padding+(row-4)*interval),4,4);
        painter.drawEllipse(QPoint(padding+(row-4)*interval,padding+(row-4)*interval),4,4);
        painter.drawEllipse(QPoint(padding+(row-4)*interval,padding+3*interval),4,4);

        if(row==19){
            painter.drawEllipse(QPoint(padding+3*interval,padding+center*interval),4,4);
            painter.drawEllipse(QPoint(padding+(row-4)*interval,padding+center*interval),4,4);
            painter.drawEllipse(QPoint(padding+center*interval,padding+3*interval),4,4);
            painter.drawEllipse(QPoint(padding+center*interval,padding+(row-4)*interval),4,4);
        }

    //画棋子
    for(int i=0;i<row ;i++){
        for(int j=0;j<row ;j++){
            StoneType current;
            if(row==19){
                current= Go->checkStone(i,j);
            }else if(row==15){
                current= Five->checkStone(i,j);
            }
            if(current==BLACK){
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(QPoint((i)*interval+padding,(j)*interval+padding),15,15);
            }
            else if(current==WHITE){
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(QPoint((i)*interval+padding,(j)*interval+padding),15,15);
            }
        }
    }
    //this->setPixmap(pix);
}
void ChessBoard::setStone(StoneType stone){
    this->stone = stone;

}
void ChessBoard::setRow(size_t row){
    this->row = row;
}
void ChessBoard::setcolumn(size_t column){
    this->column = column;
}
StoneType ChessBoard::getStone(){
    return stone;
    qDebug()<<stone;
}
void ChessBoard::putStone(size_t x, size_t y,StoneType stone){
    if(this->row==19){
       Go->putStone(x,y,stone);
    }else if(this->row==15){
        Five->putStone(x,y,stone);
    }
    update();
}

void ChessBoard::removeStone(size_t x, size_t y){
        Go->removeStone(x,y);
        update();
}

void ChessBoard::setCurrentStone(StoneType stone){
    this->currentstone=stone;
}
StoneType ChessBoard::getCurrentStone(){
    return currentstone;
}
