#ifndef APP_H
#define APP_H

#include <QApplication>
#include <QThread>
#include <QScopedPointer>

#include "mainwindow.h"
#include "projecttools.h"
#include "projectmanager.h"
#include "pycode.h"

class App : public QApplication
{
    Q_OBJECT
public:
    App(int argc, char *argv[]);
    ~App();
    void setMainWindow(MainWindow* mainWindow);

    pycode::Worker *pythonWorker;

Q_SIGNALS:

public Q_SLOTS:
    void sendEditorCodeToPython(QString);
    void sendFigureToMainWindow(const QString &figureBase64);

private:
    ProjectTools *tools;
    ProjectManager *projectManager;
    MainWindow *mainWindow;
};

#endif // APP_H
