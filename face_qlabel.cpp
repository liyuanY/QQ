#include "face_qlabel.h"
#include <QLabel>

Face_QLabel::Face_QLabel(QWidget *parent):QLabel(parent)
{
}

void Face_QLabel::mousePressEvent(QMouseEvent *ev)
{
    emit clicked();
}
