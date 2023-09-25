#ifndef APP_H
#define APP_H

#include <QApplication>
#include <QThread>

#include "mainwindow.h"
#include "projecttools.h"
#include "projectmanager.h"
#include "pythonthread.h"

class App : public QApplication
{
    Q_OBJECT
public:
    App(int argc, char *argv[]);
    ~App();
    void setMainWindow(MainWindow* mainWindow);

    PythonThread *pythonThread;

Q_SIGNALS:

public Q_SLOTS:

private:
    ProjectTools *tools;
    ProjectManager *projectManager;
};

#endif // APP_H
