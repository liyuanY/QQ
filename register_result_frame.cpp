#include "register_result_frame.h"
#include "ui_register_result_frame.h"

Register_Result_Frame::Register_Result_Frame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register_Result_Frame)
{
    ui->setupUi(this);
}

Register_Result_Frame::~Register_Result_Frame()
{
    delete ui;
}
void Register_Result_Frame::show_result(QString user_id,QString user_name,QString user_pwd)
{
    ui->textEdit->append("user_id:"+user_id);
    ui->textEdit->append("user_name:"+user_name);
    ui->textEdit->append("user_pwd:"+user_pwd);
}
