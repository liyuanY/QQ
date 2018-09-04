#include "recv_file_frame.h"
#include "ui_recv_file_frame.h"
#include <QTime>

Recv_File_Frame::Recv_File_Frame(QWidget *parent,QString user_id,QString friend_id,
                                 QString send_file_name,qint64 send_file_size) :
    QWidget(parent),
    ui(new Ui::Recv_File_Frame)
{
    ui->setupUi(this);

    setWindowTitle(user_id);
    this->user_id = user_id;
    this->friend_id = friend_id;
    this->recv_file_name = send_file_name;
    this->recv_file_size = send_file_size;
    ui->display_file_info->append("file name:"+send_file_name);
    ui->display_file_info->append("file size:"+QString::number(send_file_size));
}

//同意接收好友发送来的文件，创建服务器端套接字，等待发送文件方连接，接收数据
void Recv_File_Frame::on_button_agree_clicked()
{
    qDebug()<<"on_button_agree_clicked";
    respond_status =1;
    tcpServer_file = NULL;
    tcpClient_file = NULL;
    ip = "192.168.1.124";

    QTime time=QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

   // port = 7777;
    port =qrand()%(32700-1024+1)+1024;
    recvSize =0;


    tcpServer_file = new QTcpServer(this);
    tcpServer_file->serverPort();
    tcpServer_file->listen(QHostAddress::Any,port);
    file.setFileName(recv_file_name);
    file.open(QIODevice::WriteOnly);

    connect(tcpServer_file,&QTcpServer::newConnection,
            [=]()
            {
              //取出建立好连接的套接字
              tcpClient_file=tcpServer_file->nextPendingConnection();
              connect(tcpClient_file,&QTcpSocket::readyRead,
                      [=]()
                      {
                           //获取对方发送内容
                           QByteArray buf = tcpClient_file->readAll();
                           //接收数据
                           qint64 len = file.write(buf);
                           recvSize +=len;
                          qDebug()<<"total"<<recv_file_size<<"recvSize"<<recvSize<<len;
                           //更新进度条
                           if(recvSize == recv_file_size)
                           {
                               emit display_file_recv_sucess(recv_file_name,recv_file_size);
                               file.close();
                               tcpClient_file->disconnectFromHost();
                               tcpClient_file->close();
                               tcpServer_file->close();
                           }

                      }
                     );
            }
          );
    emit(respond_recv_file_status(respond_status,user_id,friend_id,ip,port));
    this->close();
}

//拒绝接收好友发送来的文件
void Recv_File_Frame::on_button_refuse_clicked()
{
    respond_status =0;
    emit(respond_recv_file_status(respond_status,user_id,friend_id,ip,port));
    this->close();
}

Recv_File_Frame::~Recv_File_Frame()
{
    delete ui;
}
