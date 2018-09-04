#include "add_friends_frame.h"
#include "ui_add_friends_frame.h"

Add_Friends_Frame::Add_Friends_Frame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Add_Friends_Frame)
{
    ui->setupUi(this);
    ui->button_add->setEnabled(false);         //设置添加按钮不可用
}

Add_Friends_Frame::~Add_Friends_Frame()
{
    delete ui;
}

void Add_Friends_Frame::on_button_sercher_clicked()
{
    QString friend_id = ui->friend_id->text();
    emit request_search_friend_info(friend_id);
}

void Add_Friends_Frame::display_search_friend_result(quint16 is_exist,QString friend_id,QString friend_name)
{
    if(is_exist == 1)
    {
        this->friend_id = friend_id;
        ui->display_result->append("friend_id:"+friend_id);
        ui->display_result->append("friend_name:"+friend_name);
        ui->button_add->setEnabled(true);                       //设置添加按钮可用
    }
    else
    {
        ui->display_result->append("this friend_id is not exist");
        this->friend_id.clear();
    }
}

void Add_Friends_Frame::on_button_add_clicked()
{
    emit request_add_friend(friend_id);
}
