#include "qq_server.h"
#include "ui_qq_server.h"
#include <QMessageBox>


QQ_Server::QQ_Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QQ_Server)
{
    ui->setupUi(this);
}

QQ_Server::~QQ_Server()
{
    delete ui;
}

void QQ_Server::on_server_link_clicked()
{
    if(ui->server_port->text().size() ==0)
    {
        QMessageBox::information(this,"NO port","Please input port of server");
    }
    else
    {
        server = new QQ_Server_Socket(0,8888);
        if (server != NULL)
        {
            ui->textEdit->append("Server has been launched");
            connect(server,SIGNAL(display_msg(QString)),this,SLOT(display_msg(QString)));
        }
    }
}

void QQ_Server::display_msg(QString msg)
{
    ui->textEdit->append(msg);
}
