#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QThread>
#include <QMessageBox>

#include "pythonthread.h"
#include "statuswidget.h"

#include <memory>
#include <string>
#include <iostream>

#include <QByteArray>
#include <QImage>
#include <QGraphicsPixmapItem>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), pythonThread{new PythonThread}, projectManager{pythonThread, this}
{
    ui->setupUi(this);

    statusWidget = new StatusWidget(ui->statusbar);

    connect(ui->codeEditor, SIGNAL(codeChanged(QString*)),statusWidget, SLOT(restartCooldown(QString*)));
    //connect(statusWidget, SIGNAL(cooldownCompleted(QString*)),pythonWorker, SLOT(runPython(QString*)));

    connect(ui->newButton, SIGNAL(clicked()), this, SLOT(handleNewBtn()));
    connect(ui->openButton, SIGNAL(clicked()), this, SLOT(handleOpenBtn()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(handleCloseBtn()));


    connect(this, SIGNAL(requestNewProject(QString&,QString&)), &projectManager, SLOT(createProjectRequested(QString&,QString&)));
    connect(this, SIGNAL(requestOpenProject(QString&)), &projectManager, SLOT(openProjectRequested(QString&)));
    connect(this, SIGNAL(requestCloseProject()), &projectManager, SLOT(closeProjectRequested()));

    connect(&projectManager, SIGNAL(projectOpened()), this, SLOT(updateEnabledStates()));
    connect(&projectManager, SIGNAL(projectClosed()), this, SLOT(updateEnabledStates()));

    pythonThread->start();
    pythonThread->appendToQueue("a=1");
    pythonThread->appendToQueue(R"(
a+=23
a-=1
    )");
}

//    {
//        py::scoped_interpreter guard {};
//        py::dict pylocals;
//        py::exec(R"(
//from platform import python_version
//print(python_version())
//import sys
//print(sys.path)
//import numpy as np
//x = np.linspace(0,100,1000)
//y= np.sin(x)
//np.savetxt('data1.txt', np.transpose([x,y]))
//import matplotlib.pyplot as plt
//import io
//import base64
//data1=np.loadtxt('data1.txt')
//fig, ax = plt.subplots(1,1)
//plt.plot(data1[:,0],data1[:,1])
//ax.legend(['specific legend'])
//ax.axis([0,100,-2,2])
//ax.set_title('data1.txt')#
//#somehow extract figure for display
//stringbytes = io.BytesIO()
//plt.savefig(stringbytes)
//stringbytes.seek(0)
//fig_base64 = base64.b64encode(stringbytes.read()).decode()
//        )", py::globals(), pylocals);
////        QString base64fig = QString::fromStdString( (*pylocals)["fig_base64"].cast<std::string>() );
//        QString base64fig = "==";

////        qInfo()<< base64fig;
//        QByteArray ba64 = base64fig.toUtf8();
////        QByteArray ba64 = QString::fromStdString(tst64).toUtf8();
//        QImage qimg = QImage::fromData(QByteArray::fromBase64(ba64));
//        QPixmap pixmap = QPixmap::fromImage(qimg);
//        ui->figureView->setPixmap(pixmap);
//        try {
//            py::exec(R"(
//print(aaab)
//            )", py::globals(), pylocals);

//        } catch( std::exception &e) {
//            qWarning()<<"some shit happend with python: " << e.what();
//        } catch (...) {
//            qWarning() << "Some bizzare shit happened when trying to run python code";
//        }
//    }




MainWindow::~MainWindow()
{
//    delete pythonWorker;
    if (pythonThread && pythonThread->isRunning()) {
        pythonThread->requestInterruption();
        pythonThread->quit();
        pythonThread->wait();
    }
    delete pythonThread;
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
    qInfo() << "works till here";
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
    bool projectOpen = projectManager.isAProjectOpen();
    ui->openButton->setEnabled(!projectOpen);
    ui->newButton->setEnabled(!projectOpen);
    ui->closeButton->setEnabled(projectOpen);
    ui->codeEditor->setEnabled(projectOpen);
}





