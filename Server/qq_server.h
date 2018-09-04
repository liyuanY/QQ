#ifndef QQ_SERVER_H
#define QQ_SERVER_H

#include <QWidget>
#include "qq_server_socket.h"

namespace Ui {
class QQ_Server;
}

class QQ_Server : public QWidget
{
    Q_OBJECT

public:
    explicit QQ_Server(QWidget *parent = 0);
    ~QQ_Server();

private slots:
    void on_server_link_clicked();
    void display_msg(QString msg);

private:
    Ui::QQ_Server *ui;
    QQ_Server_Socket *server;
};

#endif // QQ_SERVER_H
