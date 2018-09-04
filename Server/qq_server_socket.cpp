#include "qq_server_socket.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "qq_client_socket.h"
#include <QTime>
#include <QCoreApplication>

QQ_Server_Socket::QQ_Server_Socket(QObject *parent, quint16 server_port) : QTcpServer(parent)
{
    usernum=0;//用户数目的初始化
    qDebug()<<server_port;

    if(!listen(QHostAddress::Any,8888))
        qDebug()<<this->errorString();
    else
    {
      db=QSqlDatabase::addDatabase("QMYSQL");
      db.setHostName("localhost");
      db.setUserName("root");
      db.setPassword("123456");
      db.setDatabaseName("QQ");

      if(!db.open())
      {
          qDebug()<<"open database fail";
          exit(0);
      }
      else
      {
          QSqlQuery query;
          query.exec("select * from user");
          while(query.next())//将用户的信息保存在
          {
             qDebug()<<query.value(0).toString()<<query.value(1).toString()<<query.value(2).toString()<<query.value(3).toInt();
              /*strcpy(user_msg[usernum].username,query.value(0).toString().toLatin1().data());
              strcpy(user_msg[usernum].passwd,query.value(1).toString().toLatin1().data());
              user_msg[usernum].status=query.value(2).toInt();
               usernum++;*/
          }
      }
    }
}


/*
 *  名称：incomingConnection(qintptr socketDescriptor)
 *  类型：槽函数
 *  功能：当有用户的链接请求到来的时候触发这个函数，创建套接字和用户进行交互
*/
void QQ_Server_Socket::incomingConnection(qintptr socketDescriptor)// 当有用户的链接请求到来的时候会触发这个函数，创建套接字和用户进行交互
{
    //1 创建用于和客户端通信的套接字对象
    QQ_Client_Socket *client_socket=new QQ_Client_Socket(this);

    //2 将建立连接的套接字描述符绑定到新建立的套接字对象上
    if( client_socket->setSocketDescriptor(socketDescriptor))
    {
        //获取对方的IP和端口
        QString ip = client_socket->peerAddress().toString();
        qint16 port = client_socket->peerPort();
        QString tmp = QString("[%1:%2]:connect success").arg(ip).arg(port);
        //显示连接成功
        emit display_msg(tmp);

        qDebug("connect success");

        //1 用户登陆
        connect(client_socket,SIGNAL(requst_user_login(QString,QString,QQ_Client_Socket*)),
                this,SLOT(verify_user_login_requst(QString,QString,QQ_Client_Socket*)));
        connect(this,SIGNAL(respond_user_login(QString,quint16)),client_socket,SLOT(respond_user_login(QString,quint16)));



        //2 获取用户信息
        connect(client_socket,SIGNAL(requst_get_user_info(QString)),this,SLOT(requst_get_user_info(QString)));
        connect(this,SIGNAL(respond_send_user_info(QString,QStringList)),
                client_socket,SLOT(respond_send_user_info(QString,QStringList)));

        //4 获取好友列表
        connect(client_socket,SIGNAL(requst_get_friendslist(QString)),this,SLOT(requst_get_friendslist(QString)));
        connect(this,SIGNAL(respond_send_friendslist(QString,QStringList,QStringList,QStringList,QStringList,QStringList)),
                client_socket,SLOT(respond_send_friendslist(QString,QStringList,QStringList,QStringList,QStringList,QStringList)));

        //3 转发消息
        connect(client_socket,SIGNAL(requst_trans_msg(QString,QString,QString)),
                this,SLOT(requst_trans_msg(QString,QString,QString)));
        connect(this,SIGNAL(respond_trans_msg(quint16,QString,QString,QString)),
                client_socket,SLOT(respond_trans_msg(quint16,QString,QString,QString)));

        //6 用户下线
        connect(client_socket,SIGNAL(requst_user_logoff(QString,QString)),
                this,SLOT(requst_user_logoff(QString,QString)));

        //7 转发消息)
        connect(client_socket,SIGNAL(requst_trans_file(QString,QString,QString,qint64)),
                this,SLOT(requst_trans_file(QString,QString,QString,qint64)));
        connect(client_socket,SIGNAL(requst_send_file_data(quint16, QString,QString,QString,qint16)),
                this,SLOT(requst_send_file_data(quint16, QString,QString,QString,qint16)));

        //10注册账号
        connect(client_socket,SIGNAL(requst_register_new_user(QString,QString)),
                this,SLOT(requst_register_new_user(QString,QString)));

        connect(this,SIGNAL(respond_register_new_user(QString,QString,QString)),
                client_socket,SLOT(respond_register_new_user(QString,QString,QString)));

        //11查询添加好友信息
        connect(client_socket,SIGNAL(request_search_friend_info(QString)),
                this,SLOT(request_search_friend_info(QString)));
        connect(this,SIGNAL(respond_search_friend_info(quint16,QString,QString)),
                client_socket,SLOT(respond_search_friend_info(quint16,QString,QString)));

        connect(client_socket,SIGNAL(request_add_friend(QString,QString)),
                this,SLOT(request_add_friend(QString,QString)));
    }
}


/*
 *  名称：verify_user_login_requst(QString new_user_login_account,QString new_user_login_pwd,QQ_Client_Socket *client_socket)
 *  类型：槽函数
 *  功能：验证客户端发送来的登陆请求，获取登陆状态码
 *       触发回应信号，由套接字给客户端发送服务器的验证结果
*/
void QQ_Server_Socket::verify_user_login_requst(QString user_id,QString user_pwd,QQ_Client_Socket *client_socket)
{

    //查询用户名是否在数据库中
    QSqlQuery squery;
    quint16 user_login_status;
    squery.exec("select * from user");

    quint16 i=0;
    quint16 j=0;
    quint16 z=0;
    while(squery.next())                                             //检查用户的信息是否再数据库中，一个一个扫描数据库中的记录；
    {
         if(strcmp(user_id.toLatin1().data(),squery.value(0).toString().toLatin1().data())==0)
         {
             i++;
             if(strcmp(user_pwd.toLatin1().data(),squery.value(1).toString().toLatin1().data())==0)
             {
                 j++;
                 if(squery.value(3).toString().toInt()==0)
                 {
                     qDebug()<<squery.value(3);
                     z++;
                 }
             }
             break;
         }
    }
    if(z == 1 )                        //允许登陆
    {
        connectionlist.insert(user_id,client_socket);  //增加本次建立的连接

        char sqlchar[50]="update user set status = 1 where user_id =";
        strcat(sqlchar,user_id.toLatin1().data());
        squery.exec(sqlchar);

        user_login_status =0;    //允许登陆
    }
    else if(j == 1)
    {
        user_login_status = 3;   //重复登陆
    }
    else if(i == 1)
    {
        user_login_status = 2;    //密码错误
    }
    else
    {
        user_login_status = 1;    //账号不存在
    }
    emit respond_user_login(user_id,user_login_status);
}


/*
 *  名称：requst_get_user_info(QString user_id)
 *  类型：槽函数
 *  功能：获取数据库中user_id的个人信息，id、name及portrait
 *       发送respond_send_user_info(user_info)信号
*/
void QQ_Server_Socket::requst_get_user_info(QString user_id)
{
    QStringList user_info;

    QString user_name;
    QString user_portrait;
    QString user_signature;
    QString user_level;
    QString user_qq_show;

    QSqlQuery squery;
    squery.exec("select * from user where user_id ='"+user_id+"'");
    while(squery.next())
    {
        user_name = squery.value(2).toString();
        user_portrait = squery.value(4).toString();
        user_signature = squery.value(5).toString();
        user_level = squery.value(6).toString();
        user_qq_show = squery.value(7).toString();
    }

    user_info<<user_id<<user_name<<user_portrait<<user_level<<user_signature<<user_qq_show;
    qDebug()<<user_id<<user_name<<user_portrait<<user_level<<user_signature<<user_qq_show;
    emit respond_send_user_info(user_id,user_info);
}


/*
 *  名称：requst_get_friendslist(QString user_login_id)
 *  类型：槽函数
 *  功能：获取数据库中user_login_id的好友列表，id、name及portrait
 *       发送respond_send_friendslist(friendslist_id,friendslist_name,friendslist_portrait,
 *                                   friendslist_signature,friendslist_chat_backgd)信号
 *
*/
void QQ_Server_Socket::requst_get_friendslist(QString user_id)
{
    //在数据库中查询new_user_login_account的好友列表
    QSqlQuery squery;
    QStringList friendslist_id;
    QStringList friendslist_name;
    QStringList friendslist_portrait;
    QStringList friendslist_signature;
    QStringList friendslist_chat_backgd;

    char sqlchar[50]="select * from  f_";
    strcat(sqlchar,user_id.toLatin1().data());
    squery.exec(sqlchar);

    while(squery.next())
    {
        friendslist_id<<squery.value(0).toString();
        friendslist_name<<squery.value(1).toString();
        friendslist_portrait<<squery.value(3).toString();
        friendslist_signature<<squery.value(4).toString();
        friendslist_chat_backgd<<squery.value(5).toString();
    }

    //回应好友列表已获取，请求客户端套接字发送
    emit respond_send_friendslist(user_id,friendslist_id,friendslist_name,friendslist_portrait,
                                  friendslist_signature,friendslist_chat_backgd);

}


void QQ_Server_Socket::requst_trans_msg(QString user_id,QString friend_id,QString msg)
{
    quint16 respond_status;
    QString friend_status;

    QSqlQuery squery;
    char sqlchar[50]="select * from user where user_id =";
    strcat(sqlchar,friend_id.toLatin1().data());
    squery.exec(sqlchar);

    while(squery.next())
    {
        friend_status = squery.value(3).toString();
        qDebug()<<friend_status;
    }

    if(friend_status == "1")           //好友在线则转发
    {
        respond_status=1;
        emit respond_trans_msg(respond_status,user_id,friend_id,msg);

        //定时避免同时到达
        QTime reachTime = QTime::currentTime().addMSecs(2);
        while(QTime::currentTime()<reachTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents,100);


        QHash <QString,QQ_Client_Socket *>::const_iterator i;

        QByteArray buffer;
        QDataStream out(&buffer,QIODevice::WriteOnly);//设置一个接口用来和客户端进行数据的传送
        out<<(quint16)5<<user_id<<friend_id<<msg;

        for(i=connectionlist.constBegin();i!=connectionlist.constEnd();i++)
        {
            if(i.key()==friend_id)
             {
                i.value()->write(buffer);
                respond_status = 1;         //发送成功
             }
        }
    }
    else
    {
        respond_status = 0;       //发送失败
        emit respond_trans_msg(respond_status,user_id,friend_id,msg);
    }
}

void QQ_Server_Socket::requst_user_logoff(QString user_id,QString user_pwd)
{
    //查询用户名是否在数据库中
    QSqlQuery squery;
    squery.exec("select * from user");

    QHash <QString,QQ_Client_Socket *>::const_iterator i;

    while(squery.next())                                             //检查用户的信息是否再数据库中，一个一个扫描数据库中的记录；
    {
         if((strcmp(user_id.toLatin1().data(),squery.value(0).toString().toLatin1().data())==0) &&
            (strcmp(user_pwd.toLatin1().data(),squery.value(1).toString().toLatin1().data())==0) &&
            (squery.value(3).toString().toInt()==1))
         {
             //用户状态置为下线
             char sqlchar[50]="update user set status = 0 where user_id =";
             strcat(sqlchar,user_id.toLatin1().data());
             squery.exec(sqlchar);

             //将用户从已连接列表中删除
             for(i=connectionlist.constBegin();i!=connectionlist.constEnd();i++)
             {
                 if(i.key()==user_id)
                  {
                     connectionlist.erase(i);
                     break;
                  }
             }
             break;
         }
    }
}


void QQ_Server_Socket::requst_trans_file(QString user_id,QString friend_id,QString send_file_name,qint64 send_file_size)
{
    //quint16 respond_status;
    QString friend_status;

    QSqlQuery squery;
    char sqlchar[50]="select * from user where user_id =";
    strcat(sqlchar,friend_id.toLatin1().data());
    squery.exec(sqlchar);

    while(squery.next())
    {
        friend_status = squery.value(3).toString();
        qDebug()<<friend_status;
    }

    if(friend_status == "1")           //好友在线则转发
    {
        QByteArray buffer;
        QDataStream out(&buffer,QIODevice::WriteOnly);//设置一个接口用来和客户端进行数据的传送
        out<<(quint16)9<<user_id<<friend_id<<send_file_name<<send_file_size;

        QHash <QString,QQ_Client_Socket *>::const_iterator i;
        for(i=connectionlist.constBegin();i!=connectionlist.constEnd();i++)
        {
            if(i.key()==friend_id)
             {
                i.value()->write(buffer);
             }
        }
    }
   /* else
    {
        respond_status = 0;       //用户不在线
        emit respond_trans_file(respond_status,user_id,friend_id,send_file_name,send_file_size);
    }*/
}

void QQ_Server_Socket::requst_send_file_data(quint16 respond_status, QString friend_id,QString user_id,QString ip,qint16 port)
{
    //quint16 respond_status;
    QString friend_status;

    QSqlQuery squery;
    char sqlchar[50]="select * from user where user_id =";
    strcat(sqlchar,friend_id.toLatin1().data());
    squery.exec(sqlchar);

    while(squery.next())
    {
        friend_status = squery.value(3).toString();
        qDebug()<<friend_status;
    }

    if(friend_status == "1")           //好友在线则转发
    {
        QByteArray buffer;
        QDataStream out(&buffer,QIODevice::WriteOnly);//设置一个接口用来和客户端进行数据的传送
        out<<(quint16)10<<respond_status<<user_id<<friend_id<<ip<<port;

        QHash <QString,QQ_Client_Socket *>::const_iterator i;
        for(i=connectionlist.constBegin();i!=connectionlist.constEnd();i++)
        {
            if(i.key()==friend_id)
             {
                i.value()->write(buffer);
             }
        }
    }
}

void QQ_Server_Socket::requst_register_new_user(QString user_name,QString user_pwd)
{
    qDebug()<<"QQ_Server_Socket::requst_register_new_user";
    QSqlQuery squery;
    QString user_id;
    QString status = "0";
    QString portrait = "1";
    QString signature ="";
    QString level = "1";
    QString qq_show ="1";

    squery.exec("select * from user order by user_id");
    while(squery.next())
    {
        user_id.clear();
        user_id = QString::number(squery.value(0).toString().toInt()+1);
    }
    QString add_user="insert into user values('"+user_id+"','"+user_pwd+"','"+user_name+"','"+status+"','"+portrait+"','"+signature+"','"+level+"','"+qq_show+"')";
    squery.exec(add_user);
    QString add_friend_list="create table f_"+user_id+"(id varchar(255),user_name varchar(255),status varchar(255), portrait varchar(255),signature varchar(255),char_background varchar(255))";
    qDebug()<<add_friend_list;
    squery.exec(add_friend_list);

    emit respond_register_new_user(user_id,user_name,user_pwd);

}


void QQ_Server_Socket::request_search_friend_info(QString friend_id)
{
    QString friend_name;
    quint16 is_exist;

    QSqlQuery squery;
    QString sqlchar = "select * from user where user_id = "+friend_id+"";
    squery.exec(sqlchar);

    if(squery.next())
    {
        is_exist =1;
        friend_name = squery.value(2).toString();
        qDebug()<<friend_name;
    }
    else
    {
        is_exist =0;
        friend_name.clear();
    }

    emit respond_search_friend_info(is_exist,friend_id,friend_name);
}


void QQ_Server_Socket::request_add_friend(QString user_id,QString friend_id)
{
    QSqlQuery squery;
    QString friend_name;
    QString friend_status;
    QString friend_portrait;
    QString friend_signature;
    QString search_user = "select * from user where user_id = "+friend_id+"";
    squery.exec(search_user);

    if(squery.next())
    {
        friend_name = squery.value(2).toString();
        friend_status = squery.value(3).toString();
        friend_portrait = squery.value(4).toString();
        friend_signature = squery.value(5).toString();

        squery.exec("select * from f_"+user_id+" where id = "+friend_id+"");
        if(squery.next())
        {
            return;
        }
        else
        {
            QString add_friend="insert into f_"+user_id+" values('"+friend_id+"','"+friend_name+"','"+friend_status+"','"+friend_portrait+"','"+friend_signature+"','1') ";
            squery.exec(add_friend);
        }
    }
    squery.clear();

}






