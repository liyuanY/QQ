#ifndef REGISTER_RESULT_FRAME_H
#define REGISTER_RESULT_FRAME_H

#include <QWidget>

namespace Ui {
class Register_Result_Frame;
}

class Register_Result_Frame : public QWidget
{
    Q_OBJECT

public:
    explicit Register_Result_Frame(QWidget *parent = 0);
    ~Register_Result_Frame();

private slots:
    void show_result(QString,QString,QString);

private:
    Ui::Register_Result_Frame *ui;
};

#endif // REGISTER_RESULT_FRAME_H
