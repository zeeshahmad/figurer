#include "app.h"

#include "externalfilescanner.h"
#include "latexfileparser.h"
#include "mainwindow.h"
#include "projectmanager.h"
#include "pythonthread.h"

App::App(int argc, char *argv[])
    :QApplication(argc,argv),
    pythonThread{new PythonThread}
{
    ExternalFileScanner *scanner = new ExternalFileScanner(this);
    scanner->addParser(new LatexFileParser(pythonThread)); //latexfileparser leak!
    ProjectFileIO *io = new ProjectFileIO(this);
    tools = new ProjectTools(io, scanner, this);
    projectManager = new ProjectManager(tools, this);

    pythonThread->start();
}

App::~App()
{
    if (pythonThread) delete pythonThread;
}

void App::setMainWindow(MainWindow *mainWindow)
{
    connect(mainWindow, SIGNAL(requestNewProject(QString&,QString&)), projectManager, SLOT(createProjectRequested(QString&,QString&)));
    connect(mainWindow, SIGNAL(requestOpenProject(QString&)), projectManager, SLOT(openProjectRequested(QString&)));
    connect(mainWindow, SIGNAL(requestCloseProject()), projectManager, SLOT(closeProjectRequested()));

    connect(projectManager, &ProjectManager::projectOpened, mainWindow, [=](QString arg){
        mainWindow->updateEnabledStates(projectManager->isAProjectOpen());
    });
    connect(projectManager, &ProjectManager::projectClosed, mainWindow, [=]() {
        mainWindow->updateEnabledStates(projectManager->isAProjectOpen());
    });
}

