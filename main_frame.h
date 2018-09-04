#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <QWidget>
#include "client_socket.h"
#include <QStandardItemModel>

namespace Ui {
class Main_Frame;
}

class Main_Frame : public QWidget
{
    Q_OBJECT

public:
    explicit Main_Frame(QWidget *parent = 0, Client_Socket *socket = 0);
    ~Main_Frame();
    void closeEvent(QCloseEvent *ev);

private slots:
    void display_user_info(QStringList user_info);                        //显示用户自己的头像及个性签名、网名等

    void display_friendslist(QStringList friendslist_id,
                             QStringList friendslist_name,
                             QStringList friendslist_portrait,
                             QStringList friendslist_signature,
                             QStringList friendslist_chat_backgd);        //主窗口显示好友信息，头像，网名等
    void display_chat_frame();                                            //显示聊天框

    void on_button_add_friend_clicked();

signals:
    void requst_get_friendslist(QString user_id);                         //请求货物朋友列表
    void close_main_frame();                                              //关闭主窗口

private:
    Ui::Main_Frame *ui;

    Client_Socket *socket;                                  //与服务器通信套接字

    QString user_id;                                        //用户id
    QString user_name;                                      //用户名
    QString user_portrait;                                  //用户头像

    QString user_signature;                                 //用户个性签名
    QString user_level;                                     //用户qq级别
    QString user_qq_show;                                   //用户qq秀

    QStringList friendslist_id;                             //好友列表ID
    QStringList friendslist_name;                           //好友列表网名
    QStringList friendslist_portrait;                       //好友列表头像
    QStringList friendslist_signature;                      //好友列表个性签名
    QStringList friendslist_chat_backgd;                    //好友列表聊天背景

    QStandardItemModel *model;

};

#endif // MAIN_FRAME_H
