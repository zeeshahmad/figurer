#ifndef APP_H
#define APP_H

#include "pythonthread.h"
#include <QApplication>
#include <QThread>

class App : public QApplication
{
    Q_OBJECT
public:
    App(int argc, char *argv[]);
    ~App();



Q_SIGNALS:

public Q_SLOTS:
};

#endif // APP_H
