#ifndef LOGIN_FRAME_H
#define LOGIN_FRAME_H

#include <QWidget>
#include "client_socket.h"
#include "main_frame.h"
#include "register_frame.h"

namespace Ui {
class Login_Frame;
}

class Login_Frame : public QWidget
{
    Q_OBJECT

public:
    explicit Login_Frame(QWidget *parent = 0);
    ~Login_Frame();

private slots:
    void on_button_login_clicked();                             //请求登陆
    void display_main_frame(QString user_id);                   //显示主窗口

    void on_button_register_clicked();

signals:
    void requst_get_user_info(QString user_id);                 //请求获取用户信息，头像，个性签名，网名等

private:
    Ui::Login_Frame *ui;

    Client_Socket *socket;                          //用于与服务器通信的套接字
    Main_Frame *mf;                                 //显示主窗口

    Register_Frame *rf;
};

#endif // LOGIN_FRAME_H
