#include "client_socket.h"

Client_Socket::Client_Socket(QObject *parent):QTcpSocket(parent)
{
    connect(this,SIGNAL(connected()),this,SLOT(requst_login()));
    connect(this,SIGNAL(readyRead()),this,SLOT(read_info()));
}

//请求登陆
void Client_Socket::requst_login()
{
    QByteArray buffer;
    QDataStream out(&buffer,QIODevice::WriteOnly);
    quint16 request_type=1;
    out<<request_type<<this->user_id<<this->user_pwd<<this->localAddress();
    this->write(buffer);
}

void Client_Socket::requst_get_user_info(QString user_id)            //请求获取本人信息
{
    qDebug("获取好友列表 ---------------------------");
    //1 定义buffer
    QByteArray buffer;

    //2 将 数据标识 请求name写入buffer
    QDataStream out(&buffer,QIODevice::WriteOnly);
    quint16 request_type=2;                                    //2 表明这个是要求server给我发我的好友列表
    qDebug()<<"get_user_info"<<request_type;
    out<<request_type<<user_id;

    //3 往服务器发送请求数据
    this->write(buffer);
}


//请求获取好友列表
void Client_Socket::requst_get_friendslist(QString user_id)
{
    qDebug("获取好友列表 ---------------------------");
    //1 定义buffer
    QByteArray buffer;

    //2 将 数据标识 请求name写入buffer
    QDataStream out(&buffer,QIODevice::WriteOnly);
    quint16 request_type=3;                                    //3 表明这个是要求server给我发我的好友列表
    qDebug()<<"get_friendslist"<<request_type;
    out<<request_type<<user_id;

    //3 往服务器发送请求数据
    this->write(buffer);
}

void Client_Socket::requst_trans_msg(QString user_id,QString friend_id,QString msg)
{
    qDebug("requst_trans_msg ---------------------------");
    //1 定义buffer
    QByteArray buffer;

    //2 将 数据标识 请求name写入buffer
    QDataStream out(&buffer,QIODevice::WriteOnly);
    quint16 request_type=4;
    qDebug()<<"requst_trans_msg"<<request_type;
    out<<request_type<<user_id<<friend_id<<msg;
    qDebug()<<"---"<<msg;
    //3 往服务器发送请求数据
    this->write(buffer);
}

void Client_Socket::requst_logoff()
{
    QByteArray buffer;
    QDataStream out(&buffer,QIODevice::WriteOnly);
    quint16 request_type=6;
    out<<request_type<<this->user_id<<this->user_pwd;
    this->write(buffer);
}

void Client_Socket::requst_trans_file(QString user_id,QString friend_id,QString send_file_name,qint64 send_file_size)
{
    qDebug("获取好友列表 ---------------------------");
    //1 定义buffer
    QByteArray buffer;

    //2 将 数据标识 请求name写入buffer
    QDataStream out(&buffer,QIODevice::WriteOnly);
    quint16 request_type=7;
    out<<request_type<<user_id<<friend_id<<send_file_name<<send_file_size;
    //3 往服务器发送请求数据
    this->write(buffer);
}

void Client_Socket::respond_recv_file_status(quint16 respond_status,QString user_id,QString friend_id,QString ip,qint16 port)
{
    //1 定义buffer
    QByteArray buffer;

    //2 将 数据标识 请求name写入buffer
    QDataStream out(&buffer,QIODevice::WriteOnly);
    quint16 request_type=9;
    out<<request_type<<respond_status<<user_id<<friend_id<<ip<<port;
    //3 往服务器发送请求数据
    this->write(buffer);
}


void Client_Socket::request_search_friend_info(QString friend_id)
{
    //1 定义buffer
    QByteArray buffer;

    //2 将 数据标识 请求name写入buffer
    QDataStream out(&buffer,QIODevice::WriteOnly);
    quint16 request_type=12;
    out<<request_type<<friend_id;
    //3 往服务器发送请求数据
    this->write(buffer);
}

void Client_Socket::request_add_friend(QString friend_id)
{
    //1 定义buffer
    QByteArray buffer;

    //2 将 数据标识 请求name写入buffer
    QDataStream out(&buffer,QIODevice::WriteOnly);
    quint16 request_type=13;
    out<<request_type<<friend_id;
    //3 往服务器发送请求数据
    this->write(buffer);
}

void Client_Socket::read_info()
{

    quint16 requst_type;            //接收标识数据的类型，

    QStringList user_info;
    QString user_id;
    QString user_name;
    QString user_pwd;

    /*QString user_portrait;
    QString user_signature;
    QString user_level;*/


    quint16 user_login_status;      //用户登陆状态
    QStringList friendslist_id;      //好友列表
    QStringList friendslist_name;
    QStringList friendslist_portrait;
    QStringList friendslist_signature;
    QStringList friendslist_chat_backgd;

    quint16 respond_trans_msg_status;
    QString friend_id;
    QString friend_name;
    QString msg;

    quint16 respond_trans_file_status;
    QString send_file_name;
    qint64  send_file_size;
    QString ip;
    qint16 port;
    quint16 is_exist;


    QDataStream in(this);          //用于读取服务器端发送过来数据的数据流
    in>>requst_type;
    qDebug()<<"requst_type is "<<requst_type;

    switch(requst_type)
    {
    case 1:
        //触发用户登陆到主窗口信号
        in>>user_login_status>>user_id;
        if(user_login_status==quint16(0))
        {
            emit (display_main_frame(user_id));           //发送一个信号前端进行用户的登录；
        }
        else if(user_login_status==quint16(1))
        {
            emit (wrong_id());
        }
        else if(user_login_status==quint16(2))
        {
            emit (wrong_pwd());
        }
        else if(user_login_status==quint16(3))
        {
            emit (repeat_login());
        }
        else
        {
            emit (login_fail());
        }
        break;

    case 2:
        in>>user_info;
        emit(display_user_info(user_info));                //发送信号源，
        break;

    case 3:
        in>>friendslist_id>>friendslist_name>>friendslist_portrait>>friendslist_signature>>friendslist_chat_backgd;
        emit(display_friendslist(friendslist_id,friendslist_name,friendslist_portrait,friendslist_signature,friendslist_chat_backgd));                //发送信号源，进行主界面窗口的显示；
        break;

    case 4:
        in>>respond_trans_msg_status>>friend_id>>msg;
        emit(display_trans_msg(respond_trans_msg_status,friend_id,msg));
        break;

    case 5:
        in>>user_id>>friend_id>>msg;
        emit(display_friend_msg(friend_id,msg));
        break;

    case 9:
        in>>friend_id>>user_id>>send_file_name>>send_file_size;
        emit(requst_recv_file(friend_id,user_id,send_file_name,send_file_size));
        break;

    case 10:
        in>>respond_trans_file_status>>friend_id>>user_id>>ip>>port;
        emit(requst_send_file_data(respond_trans_file_status,friend_id,user_id,ip,port));
        break;

    case 11:
        in>>user_id>>user_name>>user_pwd;
        emit(show_register_result(user_id,user_name,user_pwd));
        break;

    case 12: //请求查询要添加好友的搜索结果
        in>>is_exist>>friend_id>>friend_name;
        emit(display_search_friend_result(is_exist,friend_id,friend_name));
        break;

    default:
        break;
    }
}

