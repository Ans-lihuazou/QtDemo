#include "mainwindowall.h"
//#include "formdoc.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindowAll w;
    w.show();
//    FormDoc w;
//    w.show();
    return a.exec();
}
