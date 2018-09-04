#ifndef REGISTER_FRAME_H
#define REGISTER_FRAME_H

#include <QWidget>
#include <QTcpSocket>
#include "client_socket.h"

namespace Ui {
class Register_Frame;
}

class Register_Frame : public QWidget
{
    Q_OBJECT

public:
    explicit Register_Frame(QWidget *parent = 0);
    ~Register_Frame();

private slots:
    void on_button_register_clicked();
    void show_register_result(QString,QString,QString);

signals:
    void show_result(QString,QString,QString);
private:
    Ui::Register_Frame *ui;
    Client_Socket  *socket;
};

#endif // REGISTER_FRAME_H
