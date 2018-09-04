#include "register_frame.h"
#include "ui_register_frame.h"
#include <QMessageBox>
#include "register_result_frame.h"

Register_Frame::Register_Frame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register_Frame)
{
    ui->setupUi(this);
}

Register_Frame::~Register_Frame()
{
    delete ui;
}

void Register_Frame::on_button_register_clicked()
{
    QString user_name = ui->user_name->text();
    QString user_pwd1 = ui->user_pwd1->text();
    QString user_pwd2 = ui->user_pwd2->text();

    if(user_name.size()==0||user_pwd1.size()==0)
   {
        QMessageBox::information(this,"NO uername","please input username or passwd");
        return;
    }
    else if(user_pwd1!=user_pwd2)
    {
         QMessageBox::information(this,"Pwd is wrong","please confirm the pwd again");
         return;
    }
    else
    {
            this->socket=new Client_Socket();
            socket->connectToHost("192.168.1.124",8888);

            QByteArray buffer;
            QDataStream out(&buffer,QIODevice::WriteOnly);////再链接以后进行信息的注册
            qDebug()<<"------69";
            out<<(quint16)11;    ////注册信息类型
            out<<user_name<<user_pwd1;
            qDebug()<<"------72";
            socket->write(buffer);
            connect(socket,SIGNAL(show_register_result(QString,QString,QString)),this,SLOT(show_register_result(QString,QString,QString)));
    }
    this->close();
}

void Register_Frame::show_register_result(QString user_id,QString user_name,QString user_pwd)
{
    Register_Result_Frame *result = new Register_Result_Frame();
    result->show();
    connect(this,SIGNAL(show_result(QString,QString,QString)),result,SLOT(show_result(QString,QString,QString)));
    emit show_result(user_id,user_name,user_pwd);
}
