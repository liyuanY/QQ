#ifndef ADD_FRIENDS_FRAME_H
#define ADD_FRIENDS_FRAME_H

#include <QWidget>

namespace Ui {
class Add_Friends_Frame;
}

class Add_Friends_Frame : public QWidget
{
    Q_OBJECT

public:
    explicit Add_Friends_Frame(QWidget *parent = 0);
    ~Add_Friends_Frame();

private slots:
    void on_button_sercher_clicked();
    void display_search_friend_result(quint16 is_exist,QString friend_id,QString friend_name);

    void on_button_add_clicked();

signals:
     void request_search_friend_info(QString);
     void request_add_friend(QString);

private:
    Ui::Add_Friends_Frame *ui;
    QString friend_id;
};

#endif // ADD_FRIENDS_FRAME_H
