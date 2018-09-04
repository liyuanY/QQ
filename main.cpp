#include "login_frame.h"
#include "main_frame.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login_Frame w;
    w.show();
    return a.exec();
}
