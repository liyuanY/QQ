#ifndef FACE_QLABEL_H
#define FACE_QLABEL_H

#include <QObject>
#include <QMouseEvent>
#include <QLabel>

class Face_QLabel:public QLabel
{
    Q_OBJECT
public:
    Face_QLabel(QWidget *parent=0);

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *ev);           // 重写这个函数实现Qlable单击事件；
};

#endif // FACE_QLABEL_H
