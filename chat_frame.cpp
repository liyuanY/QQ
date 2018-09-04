#include "chat_frame.h"
#include "ui_chat_frame.h"
#include <QDateTime>
#include <QDebug>
#include "face_frame.h"
#include <QFileDialog>
#include <QFile>
#include "recv_file_frame.h"

Chat_Frame::Chat_Frame(QWidget *parent,QString user_id,
                       QString user_name,QString friend_id,
                       QString friend_name,QString friend_portrait,
                       QString friend_signature,QString chat_background,
                       QString qq_show) :
    QWidget(parent),
    ui(new Ui::Chat_Frame)
{
    ui->setupUi(this);

    //1 保存聊天框所用资源
    this->user_name = user_name;
    this->user_id   = user_id;

    this->friend_name = friend_name;
    this->friend_id = friend_id;
    this->friend_portrait = friend_portrait;

    this->friend_signature = friend_signature;
    this->friend_chat_background = chat_background;
    this->qq_show = qq_show;

    this->qq_face=NULL;


    //2 字体大小设置
    ui->font_size->insertItem(0,tr("8"));
    ui->font_size->insertItem(1,tr("9"));
    ui->font_size->insertItem(2,tr("10"));
    ui->font_size->insertItem(3,tr("11"));
    ui->font_size->insertItem(4,tr("12"));
    ui->font_size->insertItem(5,tr("13"));
    ui->font_size->insertItem(6,tr("14"));
    ui->font_size->insertItem(7,tr("15"));
    ui->font_size->insertItem(8,tr("16"));
    ui->font_size->insertItem(9,tr("17"));
    ui->font_size->insertItem(10,tr("18"));
    ui->font_size->insertItem(11,tr("19"));
    ui->font_size->insertItem(12,tr("20"));

    //3显示头像
    QPixmap image =":/resorces/resorces/"+friend_portrait+".bmp";
    ui->label_friend_portrait->setPixmap(image);
    ui->label_friend_name->setText(friend_name);
}


void Chat_Frame::display_trans_msg(quint16 respond_trans_msg_status,
                                   QString friend_id,QString msg)
{
    if(this->friend_id == friend_id)
    {
        qDebug()<<respond_trans_msg_status;
        if(respond_trans_msg_status == 1)
        {
            ui->display_box->setTextColor(Qt::blue);
        }
        else
        {
            ui->display_box->setTextColor(Qt::red);
        }
        //获取发送消息时的时间
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        //将本人账号 发送时间 发送的消息内容 显示在对话栏中
        ui->display_box->setAlignment(Qt::AlignRight);

        ui->display_box->setCurrentFont(QFont("Times New Roman",11));
        ui->display_box->append("[ " +user_name+" ] "+ time);
        ui->display_box->append(msg);
        ui->display_box->append(QString("         "));

        //刷新发送消息栏，并将光标置为消息栏
        ui->dialog_box->clear();
        ui->dialog_box->setFocus();
    }
}

void Chat_Frame::display_friend_msg(QString friend_id,QString msg)
{
    //获取发送消息时的时间
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    //将本人账号 发送时间 发送的消息内容 显示在对话栏中
    ui->display_box->setAlignment(Qt::AlignLeft);
    ui->display_box->setTextColor(Qt::magenta);
    ui->display_box->setCurrentFont(QFont("Times New Roman",11));
    ui->display_box->append("[ " +friend_name+" ] "+ time);
    ui->display_box->append(msg);
    ui->display_box->append(QString("         "));

    //刷新发送消息栏，并将光标置为消息栏
    ui->dialog_box->clear();
    ui->dialog_box->setFocus();
}


/*
 *  名称：on_button_send_msg_clicked()
 *  类型：槽函数
 *  功能：请求转发对话框中的msg
*/
void Chat_Frame::on_button_send_msg_clicked()
{
    //获取对话框中的文本字符串，表情为“？”
    msg +=ui->dialog_box->toPlainText();//获取要发送的信息，只能获取文本
    emit(requst_trans_msg(user_id,friend_id,msg));

    //清空msg，用于下次信息发送，且同时清空对话框
    msg.clear();
    ui->dialog_box->clear();
}


/*
 *  名称：on_button_qqface_clicked()
 *  类型：槽函数
 *  功能：点击QQ表情图标，打开QQ表情面板
*/
void Chat_Frame::on_button_qqface_clicked()
{
    //判断表情框是否已经打开，防止多次重复打开
    if(qq_face != NULL)
    {
        return;
    }

    //定义QQ表情框对象 并显示
    qq_face=new Face_Frame();
    qq_face->show();

    //注册信号
    connect(qq_face,SIGNAL(display_face_in_dialogbox(QString)),
            this,SLOT(display_face_in_dialogbox(QString)));
}


/*
 *  名称：display_face_in_dialogbox(QString clickedname)
 *  类型：槽函数
 *  功能：对话框中显示qq表情
*/
void Chat_Frame::display_face_in_dialogbox(QString clickedname)
{

    QStringList array;
    QString qqface_name;

    array=clickedname.split("_");                               //字符串分割，将对应的图片资源加载到聊天窗口
    qqface_name=array.at(1);
    ui->dialog_box->insertHtml("<img src=':/resorces/qqface/"+qqface_name+".gif'>");
    msg += "<img src=':/resorces/qqface/"+qqface_name+".gif'>";
}


/*
 *  名称：on_fontComboBox_currentFontChanged(const QFont &f)
 *  类型：槽函数
 *  功能：根据聊天面板字体选择，显示聊天框字体
*/
void Chat_Frame::on_fontComboBox_currentFontChanged(const QFont &f)
{
    ui->display_box->setCurrentFont(f);
    ui->dialog_box->setCurrentFont(f);
    ui->button_send_msg->setFocus();
}


/*
 *  名称：on_front_size_currentIndexChanged(const QString &arg1)
 *  类型：槽函数
 *  功能：根据聊天面板字体选择大小，显示聊天框字体大小
*/
void Chat_Frame::on_font_size_currentIndexChanged(const QString &arg1)
{
    ui->display_box->setFontPointSize(arg1.toDouble());
    ui->dialog_box->setFontPointSize(arg1.toDouble());
    ui->dialog_box->setFocus();
}


//选择所要发送的文件，并请求发送
void Chat_Frame::on_button_send_file_clicked()
{
    //获取文件路径
    send_file_path = QFileDialog::getOpenFileName(this,"open","../");

    //判断选择文件路径是否出错
    if(send_file_path.isEmpty())
    {
        //对话框中显示打开文件出错
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        //对端发送过来的数据，放在显示栏
        ui->display_box->setAlignment(Qt::AlignRight);
        ui->display_box->setTextColor(Qt::black);
        ui->display_box->setCurrentFont(QFont("Times New Roman",11));
        ui->display_box->append("[ " +user_name+" ] "+ time);
        ui->display_box->append("fail to open the file.");
        ui->display_box->append(QString("         "));
        ui->dialog_box->setFocus();
    }
    else
    {
        send_file_name.clear();
        send_file_size = 0;
        //获取文件信息
        QFileInfo info(send_file_path);
        send_file_name = info.fileName();
        send_file_size = info.size();
        qDebug()<<send_file_size;

        //只读方式打开
        //指定文件名字
        file.setFileName(send_file_path);

        //打开文件 只读方式
        bool isok = file.open(QIODevice::ReadOnly);
        if(!isok)
        {
            //对话框中显示打开文件出错
            QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

            //对端发送过来的数据，放在显示栏
            ui->display_box->setAlignment(Qt::AlignRight);
            ui->display_box->setTextColor(Qt::red);
            ui->display_box->setCurrentFont(QFont("Times New Roman",11));
            ui->display_box->append("[ " +user_name+" ] "+ time);
            ui->display_box->append("文件打开失败.");
            ui->display_box->append(QString("         "));
            ui->dialog_box->setFocus();
        }
        else
        {
            emit(requst_trans_file(user_id,friend_id,send_file_name,send_file_size));
        }
    }
}

//好友发来文件，显示接收文件选择窗口
void Chat_Frame::display_recv_file_frame(QString friend_id,QString user_id,
                        QString send_file_name,qint64 send_file_size)
{
    if(this->friend_id == friend_id)
    {
        Recv_File_Frame *recv_file = new Recv_File_Frame(0,user_id,friend_id,
                                                         send_file_name,send_file_size);
        recv_file->show();
        connect(recv_file,SIGNAL(respond_recv_file_status(quint16,QString,QString,QString,qint16)),
                this,SLOT(respond_recv_file_status(quint16,QString,QString,QString,qint16)));
        connect(recv_file,SIGNAL(display_file_recv_sucess(QString,qint64)),
                this,SLOT(display_file_recv_sucess(QString,qint64)));

    }
}

//回应接收文件状态，若拒绝在显示窗口中显示
void Chat_Frame::respond_recv_file_status(quint16 respond_status,QString user_id,QString friend_id,QString ip,qint16 port)
{
    if(respond_status == 0)
    {
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        ui->display_box->setAlignment(Qt::AlignRight);
        ui->display_box->setCurrentFont(QFont("Times New Roman",11));
        ui->display_box->append("[ " +user_name+" ] "+ time);
        ui->display_box->setTextColor(Qt::blue);
        ui->display_box->append(send_file_name+":"+QString::number(send_file_size)+"byte,refuse to recive file");
        ui->display_box->append(QString("         "));
        ui->dialog_box->setFocus();
    }
    emit respond_recv_status(respond_status,user_id,friend_id,ip,port);
}

//连接接收文件方套接字，准备发送文件数据
void Chat_Frame::requst_send_file_data(quint16 respond_status,QString friend_id,QString user_id,QString ip,qint16 port)
{
   if(this->friend_id == friend_id)
   {
       if(respond_status ==1)
       {
           //创建通信套接字
           tcpSocket_client = new QTcpSocket(this);
           //主动与服务器建立连接
           tcpSocket_client->connectToHost(QHostAddress(ip),port);

           //连接成功，通信套接字触发connect
           connect(tcpSocket_client,SIGNAL(connected()),this,SLOT(send_data()));
       }
       else
       {
           QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
           ui->display_box->setAlignment(Qt::AlignRight);
           ui->display_box->setCurrentFont(QFont("Times New Roman",11));
           ui->display_box->append("[ " +user_name+" ] "+ time);
           ui->display_box->setTextColor(Qt::blue);
           ui->display_box->append(send_file_name+":"+QString::number(send_file_size)+"byte,friend refuse to recive");
           ui->display_box->append(QString("         "));
           ui->dialog_box->setFocus();
       }

   }
}

//发送文件数据
void Chat_Frame::send_data()
{

    sent_file_size = 0;
    quint64 len = 0;

    do
    {
        //每次发送数据大小
        char buf[1024*4] = {0};
        //往文件中读数据
        len = file.read(buf,sizeof(buf));
        //发送数据，读多少，发多少
        tcpSocket_client->write(buf,len);
        sent_file_size += len;
        qDebug()<<sent_file_size;
    }
    while(len > 0);

    //文件是否发送完毕
    if(sent_file_size == send_file_size)
    {
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        ui->display_box->setAlignment(Qt::AlignRight);
        ui->display_box->setCurrentFont(QFont("Times New Roman",11));
        ui->display_box->append("[ " +user_name+" ] "+ time);
        ui->display_box->setTextColor(Qt::blue);
        ui->display_box->append(send_file_name+":"+QString::number(send_file_size)+"byte,send success");
        ui->display_box->append(QString("         "));
        ui->dialog_box->setFocus();
        qDebug()<<"OK";
        file.close();
        tcpSocket_client->close();
    }
}

//显示接收文件成功
void Chat_Frame::display_file_recv_sucess(QString,qint64)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->display_box->setAlignment(Qt::AlignLeft);
    ui->display_box->setCurrentFont(QFont("Times New Roman",11));
    ui->display_box->append("[ " +friend_name+" ] "+ time);
    ui->display_box->setTextColor(Qt::magenta);
    ui->display_box->append(send_file_name+":"+QString::number(send_file_size)+"byte,recv success");
    ui->display_box->append(QString("         "));
    ui->dialog_box->setFocus();
}

//点击关闭按钮，关闭聊天框，如果有表情框打开就关闭表情框
void Chat_Frame::on_button_close_chat_clicked()
{
    if(qq_face!=NULL)
    {
        qq_face->close();
        delete qq_face;
    }
    this->close();
}

//主界面关闭，关闭聊天界面
void Chat_Frame::close_chat_frame()
{
    this->close();
}

//聊天界面关闭，关闭qq表情界面
void Chat_Frame::closeEvent(QCloseEvent *ev)
{
    qDebug()<<"Chat_Frame::closeEvent";
    if(qq_face!=NULL)
    {
        qq_face->close();
        delete qq_face;
    }
}

Chat_Frame::~Chat_Frame()
{
    delete ui;
}

