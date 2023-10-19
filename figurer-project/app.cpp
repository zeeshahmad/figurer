#include "app.h"

#include "externalfilescanner.h"
#include "latexfileparser.h"
#include "mainwindow.h"
#include "projectmanager.h"
#include "pycode.h"


App::App(int argc, char *argv[])
    :QApplication(argc,argv),
    pythonWorker{new pycode::Worker}
{
    ExternalFileScanner *scanner = new ExternalFileScanner(this);
    scanner->addParser(new LatexFileParser(pythonWorker)); //latexfileparser leak!
    ProjectFileIO *io = new ProjectFileIO(this);
    tools = new ProjectTools(io, scanner, this);
    projectManager = new ProjectManager(tools, this);

    importPythonCode();

    pythonWorker->start();
}

App::~App()
{
    delete pythonWorker;
}

void App::setMainWindow(MainWindow *mainWindow)
{
    connect(mainWindow->statusWidget, SIGNAL(cooldownCompleted(QString)),this, SLOT(sendEditorCodeToPython(QString)));

    connect(mainWindow, SIGNAL(requestNewProject(QString&,QString&)), projectManager, SLOT(createProjectRequested(QString&,QString&)));
    connect(mainWindow, SIGNAL(requestOpenProject(QString&)), projectManager, SLOT(openProjectRequested(QString&)));
    connect(mainWindow, SIGNAL(requestCloseProject()), projectManager, SLOT(closeProjectRequested()));

    connect(projectManager, &ProjectManager::projectOpened, mainWindow, [=](QString arg){
        mainWindow->updateEnabledStates(projectManager->isAProjectOpen());
    });
    connect(projectManager, &ProjectManager::projectClosed, mainWindow, [=]() {
        mainWindow->updateEnabledStates(projectManager->isAProjectOpen());
    });
    this->mainWindow = mainWindow;
}

void App::sendEditorCodeToPython(QString newPythonCode)
{
    qInfo() << "send code to python";

    newPythonCode.prepend(pythonFunctionCode);

    pythonWorker->cancelCodesWithTag("editor");
    pythonWorker->enqueue(newPythonCode, "figure_image_data", "editor")
        .then([this](pycode::Result result){
            QSharedPointer<QByteArray> figureImageData = QSharedPointer<QByteArray>::create(result.toByteArray());
            sendFigureToMainWindow(figureImageData);
        });
}

void App::sendFigureToMainWindow(QSharedPointer<QByteArray> figureImageData)
{
    mainWindow->updateFigureView(figureImageData);
}

void App::importPythonCode()
{
    QFile figureImageDataCodeFile(":/python/figure_image_data.py");
    if (!figureImageDataCodeFile.open(QIODevice::ReadOnly|QIODevice::Text)) {
        qInfo()<< "Could not open figure image data file to append python function";
        return;
    }
    pythonFunctionCode = QString::fromUtf8(figureImageDataCodeFile.readAll());
    figureImageDataCodeFile.close();
}
