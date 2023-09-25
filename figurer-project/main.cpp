
#include "mainwindow.h"
#include "app.h"


int main(int argc, char *argv[])
{
    App a(argc, argv);
    MainWindow w;
    a.setMainWindow(&w);
    w.show();
    return a.exec();
}
