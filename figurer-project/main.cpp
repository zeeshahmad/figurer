
#include "mainwindow.h"
#include "app.h"


#include "pybind11/embed.h"
namespace py=pybind11;
int main(int argc, char *argv[])
{

    App a(argc, argv);
    MainWindow w;
    a.setMainWindow(&w);
    w.show();
    return a.exec();
}
