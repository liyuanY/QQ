#include "qq_client_socket.h"
#include <QDataStream>
#include <QByteArray>
#include <QStringList>

QQ_Client_Socket::QQ_Client_Socket(QObject *parent) : QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(client_requst_coming()));
}

void QQ_Client_Socket::client_requst_coming()
{
    qDebug("messge comming!!!,you should receive it!");

    quint16 requst_type;             //首先检测

    QString user_id;
    QString user_name;
    QString user_pwd;
    QHostAddress user_login_addr;

    QString friend_id;               //信息接受者
    QString msg;                     //信息的具体内容

    QString send_file_name;
    qint64 send_file_size;
    quint16 respond_recv_file_status;

    QString ip;
    qint16 port;

    //QString register_user_name;          //注册信息的用户名
    //QString register_user_pwd;           //注册信息的用户密码

    QByteArray buffer;
    QDataStream in(this);
    QDataStream out(&buffer,QIODevice::WriteOnly);//设置一个接口用来和客户端进行数据的传送

    in>>requst_type;
    qDebug()<<requst_type;


    switch(requst_type)
    {
    case 1:        //新用户登陆
        in>>user_id>>user_pwd>>user_login_addr;
        this->user_id=user_id;
        emit(requst_user_login(user_id,user_pwd,this));
        break;

    case 2:        //获取本人信息
        in>>user_id;
        emit(requst_get_user_info(user_id));
        break;

    case 3:        //获取好友列表
        in>>user_id;
        emit(requst_get_friendslist(user_id));
        break;

    case 4:      //转发消息
        in>>user_id>>friend_id>>msg;
        qDebug()<<user_id<<friend_id<<msg;
        emit(requst_trans_msg(user_id,friend_id,msg));
        qDebug()<<msg;
        break;

    case 6:       //用户下线
        in>>user_id>>user_pwd;
        emit(requst_user_logoff(user_id,user_pwd));
        break;

    case 7:      //请求转发文件给好友
        in>>user_id>>friend_id>>send_file_name>>send_file_size;
        emit(requst_trans_file(user_id,friend_id,send_file_name,send_file_size));
        break;

    case 9:     //回应好友同意接收文件
       in>>respond_recv_file_status>>user_id>>friend_id>>ip>>port;
       emit requst_send_file_data(respond_recv_file_status,friend_id,user_id,ip,port);
       break;

    case 11:     //注册账号
       in>>user_name>>user_pwd;
       emit requst_register_new_user(user_name,user_pwd);
       break;

     case 12:
        in>>friend_id;
        emit request_search_friend_info(friend_id);

    case 13:
        in>>friend_id;
        emit request_add_friend(this->user_id,friend_id);

    default:
        break;
    }
}


void QQ_Client_Socket::respond_user_login(QString user_id,quint16 user_login_status)
{
    if(this->user_id == user_id)
    {
        //1 定义buffer
        QByteArray buffer;

        //2 将 数据标识写入buffer
        QDataStream out(&buffer,QIODevice::WriteOnly);
        quint16 requst_type = 1;
        qDebug()<<"respond_user_login_requst"<<requst_type<<user_login_status<<this->user_id;
        out<<requst_type<<user_login_status<<this->user_id;

        //3 往服务器发送请求数据
        this->write(buffer);
    }
}


void QQ_Client_Socket::respond_send_user_info(QString user_id,QStringList user_info)
{
    if(this->user_id == user_id)
    {
        //1 定义buffer
        QByteArray buffer;

        //2 将 数据标识写入buffer
        QDataStream out(&buffer,QIODevice::WriteOnly);
        quint16 requst_type = 2;

        qDebug()<<"respond_get_friendslist_requst"<<requst_type;
        out<<requst_type<<user_info;

        //3 往服务器发送请求数据
        this->write(buffer);
    }
}
void QQ_Client_Socket::respond_send_friendslist(QString user_id,QStringList friendslist_id,
                                                QStringList friendslist_name,
                                                QStringList friendslist_portrait,
                                                QStringList friendslist_signature,
                                                QStringList friendslist_chat_backgd)
{
    if(this->user_id == user_id)
    {
        //1 定义buffer
        QByteArray buffer;

        //2 将 数据标识写入buffer
        QDataStream out(&buffer,QIODevice::WriteOnly);
        quint16 requst_type = 3;

        qDebug()<<"respond_get_friendslist_requst"<<requst_type;
        out<<requst_type<<friendslist_id<<friendslist_name<<friendslist_portrait<<friendslist_signature<<friendslist_chat_backgd;

        //3 往服务器发送请求数据
        this->write(buffer);
    }
}


void QQ_Client_Socket::respond_trans_msg(quint16 respond_status,QString user_id,QString recv_id,QString msg)
{
    //1 定义buffer
    QByteArray buffer;

    //2 将 数据标识写入buffer
    QDataStream out(&buffer,QIODevice::WriteOnly);
    quint16 requst_type = 4;

    qDebug()<<"respond_trans_msg"<<requst_type;
    out<<requst_type<<respond_status<<recv_id<<msg;

    //3 往服务器发送请求数据
    this->write(buffer);
}

void QQ_Client_Socket::respond_register_new_user(QString user_id,QString user_name,QString user_pwd)
{
    //1 定义buffer
    QByteArray buffer;

    //2 将 数据标识写入buffer
    QDataStream out(&buffer,QIODevice::WriteOnly);
    quint16 requst_type = 11;
    out<<requst_type<<user_id<< user_name<<user_pwd;

    //3 往服务器发送请求数据
    this->write(buffer);
}

void QQ_Client_Socket::respond_search_friend_info(quint16 is_exist,QString friend_id,QString friend_name)
{
    //1 定义buffer
    QByteArray buffer;

    //2 将 数据标识写入buffer
    QDataStream out(&buffer,QIODevice::WriteOnly);
    quint16 requst_type = 12;
    out<<requst_type<<is_exist<< friend_id<<friend_name;

    //3 往服务器发送请求数据
    this->write(buffer);
}
