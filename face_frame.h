#ifndef FACE_FRAME_H
#define FACE_FRAME_H

#include <QWidget>

namespace Ui {
class Face_Frame;
}

class Face_Frame : public QWidget
{
    Q_OBJECT

public:
    explicit Face_Frame(QWidget *parent = 0);
    ~Face_Frame();

private slots:
    void face_qlable_clicked();                            //qq表情被点击

signals:
    void display_face_in_dialogbox(QString beclicked_name);

private:
    Ui::Face_Frame *ui;
};

#endif // FACE_FRAME_H
