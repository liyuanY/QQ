#include "qq_server.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QQ_Server w;
    w.show();

    return a.exec();
}
