#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QThread>
#include <QMessageBox>

#include "latexfileparser.h"
#include "pythonthread.h"
#include "statuswidget.h"

#include <memory>
#include <string>
#include <iostream>

#include <QByteArray>
#include <QImage>
#include <QGraphicsPixmapItem>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
        ui(new Ui::MainWindow),
        pythonThread{new PythonThread}
{

    ExternalFileScanner *scanner = new ExternalFileScanner(this);
    scanner->addParser(new LatexFileParser(pythonThread)); //latexfileparser leak!
    ProjectFileIO *io = new ProjectFileIO(this);
    tools = new ProjectTools(io, scanner, this);
    projectManager = new ProjectManager(tools, this);

    ui->setupUi(this);

    statusWidget = new StatusWidget(ui->statusbar);

    connect(ui->codeEditor, SIGNAL(codeChanged(QString*)),statusWidget, SLOT(restartCooldown(QString*)));
    //connect(statusWidget, SIGNAL(cooldownCompleted(QString*)),pythonWorker, SLOT(runPython(QString*)));

    connect(ui->newButton, SIGNAL(clicked()), this, SLOT(handleNewBtn()));
    connect(ui->openButton, SIGNAL(clicked()), this, SLOT(handleOpenBtn()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(handleCloseBtn()));

    connect(this, SIGNAL(requestNewProject(QString&,QString&)), projectManager, SLOT(createProjectRequested(QString&,QString&)));
    connect(this, SIGNAL(requestOpenProject(QString&)), projectManager, SLOT(openProjectRequested(QString&)));
    connect(this, SIGNAL(requestCloseProject()), projectManager, SLOT(closeProjectRequested()));

    connect(projectManager, SIGNAL(projectOpened(QString)), this, SLOT(updateEnabledStates()));
    connect(projectManager, SIGNAL(projectClosed()), this, SLOT(updateEnabledStates()));

    updateEnabledStates();

    pythonThread->start();

}

MainWindow::~MainWindow()
{
//    if (pythonThread && pythonThread->isRunning()) {
//        pythonThread->requestInterruption();
//        pythonThread->quit();
//        pythonThread->wait();
//    }
//    delete pythonThread;
    if (pythonThread) delete pythonThread;
    delete ui;
}

void MainWindow::show_error(const QString &message)
{
    QMessageBox messageBox;
    messageBox.critical(0,"Error",message);
    messageBox.setFixedSize(500,200);
}

void MainWindow::handleNewBtn()
{
    QString saveFilePath = QFileDialog::getSaveFileName(this, "Choose project location and name", nullptr, "Figurer project (*.json)");
    if (saveFilePath.isNull()) return;
    qInfo()<<"saveFilePath: " << saveFilePath;
    QString externalFilePath = QFileDialog::getOpenFileName(this, "Choose the document that will contain figures", nullptr, "LaTeX document (*.tex);; Word document (*.docx)");
    if (externalFilePath.isNull()) return;
    qInfo() <<"externalFilePath: "<< externalFilePath;
    Q_EMIT requestNewProject(saveFilePath, externalFilePath);

}



void MainWindow::handleOpenBtn()
{
    QString openFilePath = QFileDialog::getOpenFileName(this, "Open project file", nullptr, "Figurer project (*.json)");
    if (openFilePath.isNull()) return; //user cancelled dialog
    qInfo() << "openFilePath: " << openFilePath;
    Q_EMIT requestOpenProject(openFilePath);

}

void MainWindow::handleCloseBtn()
{
    Q_EMIT requestCloseProject();
}

void MainWindow::updateEnabledStates()
{
    bool projectOpen = projectManager->isAProjectOpen();
    ui->openButton->setEnabled(!projectOpen);
    ui->newButton->setEnabled(!projectOpen);
    ui->closeButton->setEnabled(projectOpen);
    ui->codeEditor->setEnabled(projectOpen);
}





