#ifndef CHAT_FRAME_H
#define CHAT_FRAME_H

#include <QWidget>
#include "face_frame.h"
#include <QFile>
#include <QTcpSocket>

namespace Ui {
class Chat_Frame;
}

class Chat_Frame : public QWidget
{
    Q_OBJECT

public:
    explicit Chat_Frame(QWidget *parent = 0,QString user_id=0,
                        QString user_name=0,QString friend_id=0,
                        QString friend_name=0,QString friend_portrait=0,
                        QString friend_signature=0,QString friend_chat_background=0,
                        QString qq_show=0);
    void closeEvent(QCloseEvent *ev);
    ~Chat_Frame();

private slots:
    void display_trans_msg(quint16,QString,QString);             //自己发送的消息
    void display_friend_msg(QString,QString);                    //好友发送过来的消息
    void on_button_send_msg_clicked();                           //发送对话框中消息

    void on_button_qqface_clicked();
    void display_face_in_dialogbox(QString);                     //对话框中显示选择的QQ表情

    void on_fontComboBox_currentFontChanged(const QFont &f);     //字体设置
    void on_font_size_currentIndexChanged(const QString &arg1);  //字体大小设置

    void on_button_send_file_clicked();
    void display_recv_file_frame(QString,QString,QString,qint64);
    void respond_recv_file_status(quint16 respond_status,QString user_id,QString friend_id,QString ip,qint16 port);
    void requst_send_file_data(quint16 respond_status,QString user_id,QString friend_ip,QString ip,qint16 port);
    void send_data();
    void display_file_recv_sucess(QString,qint64);

    void on_button_close_chat_clicked();
    void close_chat_frame();

signals:
    void requst_trans_msg(QString user_id,QString friend_id,QString msg);

    void respond_recv_status(quint16 respond_status,QString user_id,QString friend_id,QString ip,qint16 port);
    void requst_trans_file(QString user_id,QString friend_id,QString send_file_name,qint64 send_file_size);


private:
    Ui::Chat_Frame *ui;

    QString user_name;               //用户名
    QString user_id;                 //用户名
    QString qq_show;

    QString friend_name;             //好友名
    QString friend_id;
    QString friend_portrait;

    QString friend_signature;        //好友签名
    QString friend_chat_background;  //聊天背景


    QString msg;
    Face_Frame *qq_face;


    //声明文件对象
    QFile file;
    QTcpSocket *tcpSocket_client;

    //声明文件名
    QString send_file_name;
    QString recv_file_name;
    QString send_file_path;

    //声明文件大小
    qint64 send_file_size;
    qint64 recv_file_size;

    //已发送文件大小 已接收文件大小
    qint64 sent_file_size;
    qint64 recved_file_size;

};

#endif // CHAT_FRAME_H
