#ifndef QQ_CLIENT_SOCKET_H
#define QQ_CLIENT_SOCKET_H
#include <QtNetwork/QTcpSocket>
#include <QObject>
#include <QHostAddress>

class QQ_Client_Socket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit QQ_Client_Socket(QObject *parent = nullptr);

signals:
    void requst_user_login(QString user_id,QString user_pwd,QQ_Client_Socket * client_socket);
    void requst_user_logoff(QString user_id,QString user_pwd);

    void requst_get_user_info(QString user_id);
    void requst_get_friendslist(QString user_id);

    void requst_trans_msg(QString send_id,QString recv_id,QString msg);
    void requst_trans_file(QString user_id,QString friend_id,QString send_file_name,qint64 send_file_size);
    void requst_send_file_data(quint16 respond_status, QString friend_id,QString user_id,QString ip,qint16 port);

    void requst_register_new_user(QString user_name,QString user_pwd);

    void request_search_friend_info(QString);
    void request_add_friend(QString user_id,QString friend_id);




public slots:
    void client_requst_coming();

    void respond_user_login(QString user_id,quint16 new_user_login_status);
    void respond_send_user_info(QString user_id,QStringList user_info);
    void respond_send_friendslist(QString user_id,
                                  QStringList friendslist_id,
                                  QStringList friendslist_name,
                                  QStringList friendslist_portrait,
                                  QStringList friendslist_signature,
                                  QStringList friendslist_chat_backgd);
    void respond_trans_msg(quint16 respond_status,QString user_id,QString recv_id,QString msg);
    void respond_register_new_user(QString,QString,QString);

    void respond_search_friend_info(quint16,QString,QString);


private:
    bool relogin;
    bool new_rgsok;

    QString user_id;
    QHostAddress addr;
};

#endif // QQ_CLIENT_SOCKET_H
