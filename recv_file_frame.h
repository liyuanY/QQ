#ifndef RECV_FILE_FRAME_H
#define RECV_FILE_FRAME_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>

namespace Ui {
class Recv_File_Frame;
}

class Recv_File_Frame : public QWidget
{
    Q_OBJECT

public:
    explicit Recv_File_Frame(QWidget *parent = 0,QString user_id =0,QString friend_id = 0,
                             QString send_file_name = 0,qint64 send_file_size = 0);
    ~Recv_File_Frame();

private slots:
    void on_button_agree_clicked();

    void on_button_refuse_clicked();

signals:
    void respond_recv_file_status(quint16,QString,QString,QString,qint16);
    void display_file_recv_sucess(QString recv_file_name,qint64 recv_file_size);

private:
    Ui::Recv_File_Frame *ui;

    QString user_id;
    QString friend_id;
    QString recv_file_name;
    qint64 recv_file_size;

    quint16 respond_status;
    QTcpServer *tcpServer_file;
    //创建通信套接字
    QTcpSocket *tcpClient_file;

    QFile file;
    quint64 recvSize;
    qint16 port;
    QString ip;

};

#endif // RECV_FILE_FRAME_H
