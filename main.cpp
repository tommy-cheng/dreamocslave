#include "dreamocslave.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dreamocslave w;
    w.show();

    return a.exec();
}
