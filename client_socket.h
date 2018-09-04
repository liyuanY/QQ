#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H
#include<QtNetwork>
#include <QHostAddress>

class Client_Socket: public QTcpSocket
{
    Q_OBJECT
public:
    Client_Socket(QObject *parent=0);

public:
    inline void set_name(QString user_id){this->user_id=user_id;}             //设置账号
    inline void set_pwd(QString user_pwd){this->user_pwd=user_pwd;}           //设置密码
    inline QString getname(){return user_id;}                                 //获取账号
    inline QString getpwd(){return user_pwd;}                                 //获取密码

private slots:
    void read_info();

    void requst_login();                                    //请求登陆
    void requst_logoff();                                   //请求下线

    void requst_get_user_info(QString user_id);             //请求获取本人信息
    void requst_get_friendslist(QString user_id);           //请求获取好友列表

    void requst_trans_msg(QString user_id,QString friend_name,QString msg);   //请求转发消息

    void requst_trans_file(QString user_id,QString friend_id,QString send_file_name,qint64 send_file_size);
    void respond_recv_file_status(quint16,QString,QString,QString,qint16);

    void request_search_friend_info(QString);
    void request_add_friend(QString);


signals:
    //登陆失败检测
    void wrong_id();
    void wrong_pwd();
    void repeat_login();
    void login_fail();

    void display_main_frame(QString user_id);

    void display_user_info(QStringList user_info);
    void display_friendslist(QStringList friendslist_id,
                             QStringList friendslist_name,
                             QStringList friendslist_portrait,
                             QStringList friendslist_signature,
                             QStringList friendslist_chat_backgd); //这个信号用来进行通知主窗口的函数来接收好友列表并进行显示；

    void display_trans_msg(quint16 respond_trans_msg_status,QString friend_id,QString msg);
    void display_friend_msg(QString friend_id,QString msg);

    void requst_recv_file(QString,QString,QString,qint64);
    void display_recv_file(QString user_id,QString friend_id,QString send_file_name,qint64 send_file_size);
    void requst_send_file_data(quint16 respond_trans_file_status,QString friend_id,QString user_id,QString ip,qint16 port);
    void show_register_result(QString,QString,QString);

    void display_search_friend_result(quint16 is_exist,QString friend_id,QString friend_name);


private:
    QString user_id;
    QString user_pwd;
};

#endif // CLIENT_SOCKET_H
