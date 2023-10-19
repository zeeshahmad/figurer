#ifndef APP_H
#define APP_H

#include <QApplication>

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
    void sendFigureToMainWindow(QSharedPointer<QByteArray> figureImageData);

private:
    ProjectTools *tools;
    ProjectManager *projectManager;
    MainWindow *mainWindow;
    QString pythonFunctionCode;

    void importPythonCode();
};

#endif // APP_H
