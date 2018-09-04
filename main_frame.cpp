#include "main_frame.h"
#include "ui_main_frame.h"
#include <QStandardItemModel>
#include "chat_frame.h"
#include "add_friends_frame.h"

Main_Frame::Main_Frame(QWidget *parent,Client_Socket *socket) :
    QWidget(parent),
    ui(new Ui::Main_Frame)
{
    ui->setupUi(this);
    this->socket = socket;
    //setWindowFlag(Qt::FramelessWindowHint);                         //设置边框隐藏
    ui->lineEdit_search->setPlaceholderText("search");

    //1 显示用户信息
    connect(this->socket,SIGNAL(display_user_info(QStringList)),
            this,SLOT(display_user_info(QStringList)));

    //2 请求 显示好友列表信息
    connect(this,SIGNAL(requst_get_friendslist(QString)),this->socket,SLOT(requst_get_friendslist(QString)));
    connect(this->socket,SIGNAL(display_friendslist(QStringList,QStringList,QStringList,QStringList,QStringList)),
            this,SLOT(display_friendslist(QStringList,QStringList,QStringList,QStringList,QStringList)));

    //3 显示聊天窗口
    connect(ui->treeView_friends,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(display_chat_frame()));

    //4 请求下线
    connect(this,SIGNAL(close_main_frame()),this->socket,SLOT(requst_logoff()));
}

/*
 *  名称：display_user_info(QStringList user_info)
 *  类型：槽函数
 *  功能：保存用户信息到主窗口对象，主界面显示用户信息，发射获取好友列表信号
*/
void Main_Frame::display_user_info(QStringList user_info)
{
    //1 保存用户信息到主窗口对象中
    this->user_id = user_info.at(0);                                   //用户ID
    this->user_name = user_info.at(1);                                 //用户名
    this->user_portrait = user_info.at(2);                             //用户头像
    this->user_level = user_info.at(3);                                //用户等级
    this->user_signature = user_info.at(4);                            //用户个性标签
    this->user_qq_show = user_info.at(5);                            //用户个性标签

    //2 将用户信息显示在主窗口
    QPixmap image =":/resorces/resorces/"+user_portrait+".bmp";
    ui->label_user_portrait->setPixmap(image);                         //显示用户头像
    ui->label_user_name->setText(user_name);                           //显示用户名
    ui->lineEdit_user_signature->setText(user_signature);              //显示用户个性签名

    //3 发射获取好友列表信号
    emit(requst_get_friendslist(user_id));
}

/*
 *  名称：display_friendslist(QStringList friendslist_id,
                             QStringList friendslist_name,
                             QStringList friendslist_portrait)
 *  类型：槽函数
 *  功能：将friendlist中的好友信息刷新到主窗口中 并 发送 信号
 *       请求服务器告知好友我上线了
*/
void Main_Frame::display_friendslist(QStringList friendslist_id,
                                     QStringList friendslist_name,
                                     QStringList friendslist_portrait,
                                     QStringList friendslist_signature,
                                     QStringList friendslist_chat_backgd)
{
    qDebug()<<"set_friendslist()----------------------";

    this->friendslist_id = friendslist_id;
    this->friendslist_name =friendslist_name;
    this->friendslist_portrait = friendslist_portrait;
    this->friendslist_signature = friendslist_signature;
    this->friendslist_chat_backgd = friendslist_chat_backgd;

    //1 初始化一个QStandardItemModel模型，配合treeView 用于显示好友列表
    model = new QStandardItemModel();
    model->setHorizontalHeaderLabels(QStringList("-------------------FriendsList----------------------"));            //设置标题栏为FriendsList
    QString image;

    //2 显示好友
    for(int i = 0;i<friendslist_name.size();i++)
    {
        //初始化一个好友对象栏，每位好友对应一个对象
        QStandardItem *itemChild = new QStandardItem(static_cast<QString>(friendslist_name.at(i)));
        //设置好友对象栏中好友头像 i+1最大134 可以用循环使图像重复可用

        image =":/resorces/resorces/"+friendslist_portrait.at(i)+".bmp";
        QIcon icon(image);
        itemChild->setIcon(icon);

        //将每一位好友对象显示在treeView中， 显示为：头像 用户昵称
        model->setItem(i,itemChild);
        ui->treeView_friends->setModel(model);
    }
}



/*
 *  名称：show_chat()
 *  类型：槽函数
 *  功能：显示与好友聊天对话框，并注册与聊天相关事件的信号
*/
void Main_Frame::display_chat_frame()
{
    //1 获取选中后好友信息
    QModelIndex index=ui->treeView_friends->currentIndex();        //获取双击选中后的好友
    QString friend_name=index.data().toString();                  //获取好友账号名
    QString friend_id;
    QString friend_portrait;
    QString friend_signature;
    QString friend_chat_background;

    qDebug()<<friend_name;
    //获取好友ID和用户名
    for(int i = 0;i<friendslist_name.size();i++)
    {
        qDebug()<<friendslist_name.at(i)<<"---"<<friend_name;
        if(friendslist_name.at(i) == friend_name)
        {
            friend_id=friendslist_id.at(i);
            friend_portrait = friendslist_portrait.at(i);
            friend_signature = friendslist_signature.at(i);
            friend_chat_background = friendslist_chat_backgd.at(i);
        }
    }

    //2 显示用于聊天的对话框
    Chat_Frame *chat=new Chat_Frame(0,user_id,user_name,friend_id,friend_name,friend_portrait,
                                    friend_signature,friend_chat_background,this->user_qq_show);  //创建聊天对象
    chat->setWindowTitle(friend_id);                        //设置聊天窗口的标题
    chat->show();                                             //显示聊天窗口

    connect(this,SIGNAL(close_main_frame()),chat,SLOT(close_chat_frame()));


    connect(chat,SIGNAL(requst_trans_msg(QString,QString,QString)),
            this->socket,SLOT(requst_trans_msg(QString,QString,QString)));                      //给好友发送消息，请求服务器转发
    connect(this->socket,SIGNAL(display_trans_msg(quint16,QString,QString)),
            chat,SLOT(display_trans_msg(quint16,QString,QString)));                             //显示给好友发送的消息，并显示成功或失败
    connect(this->socket,SIGNAL(display_friend_msg(QString,QString)),
            chat,SLOT(display_friend_msg(QString,QString)));                                    //显示好友发送过来的数据

    connect(chat,SIGNAL(requst_trans_file(QString,QString,QString,qint64)),
            this->socket,SLOT(requst_trans_file(QString,QString,QString,qint64)));                      //给好友发送文件，请求服务器转发
    connect(this->socket,SIGNAL(requst_recv_file(QString,QString,QString,qint64)),                      //请求接收文件，弹出接收选择框
            chat,SLOT(display_recv_file_frame(QString,QString,QString,qint64)));
    connect(chat,SIGNAL(respond_recv_status(quint16,QString,QString,QString,qint16)),                   //回应是否同意接收文件
            this->socket,SLOT(respond_recv_file_status(quint16,QString,QString,QString,qint16)));
    connect(this->socket,SIGNAL(requst_send_file_data(quint16,QString,QString,QString,qint16)),         //如果接收端同意，开始发送文件数据
            chat,SLOT(requst_send_file_data(quint16,QString,QString,QString,qint16)));
}

void Main_Frame::closeEvent(QCloseEvent *ev)
{
    qDebug()<<"Chat_Frame::closeEvent";
    emit close_main_frame();
}

Main_Frame::~Main_Frame()
{
    delete ui;
}

void Main_Frame::on_button_add_friend_clicked()
{
    Add_Friends_Frame *add_friend = new Add_Friends_Frame();
    add_friend->show();
    connect(add_friend,SIGNAL(request_search_friend_info(QString)),this->socket,SLOT(request_search_friend_info(QString)));
    connect(this->socket,SIGNAL(display_search_friend_result(quint16,QString,QString)),
            add_friend,SLOT(display_search_friend_result(quint16,QString,QString)));

    connect(add_friend,SIGNAL(request_add_friend(QString)),this->socket,SLOT(request_add_friend(QString)));
}
