#include "login_frame.h"
#include "ui_login_frame.h"
#include <QMessageBox>
#include "main_frame.h"
#include "register_frame.h"

Login_Frame::Login_Frame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login_Frame)
{

    ui->setupUi(this);
    ui->lineEdit_user_id->setFocus();                           //1 设置光标处在输入账号位置
    ui->lineEdit_user_pwd->setEchoMode(QLineEdit::Password);   //2 设置密码不可见
    this->setWindowTitle("QQ");                               //3 设置登陆窗口标题
}



void Login_Frame::on_button_login_clicked()
{
    //1 创建用于与服务器通信的套接字，并连接服务器
    qDebug("Prepare to connect server!!!");
    socket = new Client_Socket(this);
    socket->connectToHost("192.168.1.124",8888);

    //2 套接字对象中保存用户账号及密码以便后期通信使用
    socket->set_name(ui->lineEdit_user_id->text());
    socket->set_pwd(ui->lineEdit_user_pwd->text());

    //3 注册信号
    connect(socket,SIGNAL(display_main_frame(QString)),this,SLOT(display_main_frame(QString)));
    connect(this,SIGNAL(requst_get_user_info(QString)),socket,SLOT(requst_get_user_info(QString)));

    connect(socket,&Client_Socket::wrong_id,
            [=]()
            {
                socket->close();
                QMessageBox::information(this,"Wrong account id！！","please enter the correct account");
            }
            );
    connect(socket,&Client_Socket::wrong_pwd,
            [=]()
            {
                socket->close();
                QMessageBox::information(this,"Wrong password！！","please enter the correct password");
            }
            );
    connect(socket,&Client_Socket::repeat_login,
            [=]()
            {
                socket->close();
                QMessageBox::information(this,"This account is logged in！！","please logoff and log back in");
            }
            );
    connect(socket,&Client_Socket::login_fail,
            [=]()
            {
                socket->close();
                QMessageBox::information(this,"Login failed","please log back in later");
            }
            );
}


void Login_Frame::display_main_frame(QString user_id)
{
    //1 关闭登录窗口；
    this->close();

    //2 显示登陆后主窗口 并将与服务器通信的套接字接收过来
    mf=new Main_Frame(0,socket);                            //将套接字转移到登陆后的主窗口；
    mf->setWindowTitle(user_id);                           //设置登录窗体的用户名；
    mf->show();

    //3 发送信号给 套接字对象 向服务器请求用户列表
    emit(requst_get_user_info(user_id));
}

Login_Frame::~Login_Frame()
{
    delete ui;
}

//注册新用户
void Login_Frame::on_button_register_clicked()
{
    rf = new Register_Frame();
    rf->show();
}
