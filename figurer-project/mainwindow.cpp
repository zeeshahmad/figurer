#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QThread>

#include "statuswidget.h"

#include <pybind11/embed.h>

#include <string>
#include <iostream>
#include <stdio.h>

#include <sstream>
#include <vector>


namespace py = pybind11;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    codeEditor = ui->textEditTest;
    statusWidget = new StatusWidget(ui->statusbar);
    pythonWorker = new PythonWorker;


    connect(codeEditor, SIGNAL(codeChanged(QString*)),statusWidget, SLOT(restartCooldown(QString*)));
    connect(statusWidget, SIGNAL(cooldownCompleted(QString*)),pythonWorker, SLOT(runPython(QString*)));
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::handleOpenLatex()
{
    QString latexpath = QFileDialog::getOpenFileName(this, "Open latex document", nullptr, "tex files (*.tex)");
    qInfo() << latexpath;
    QFile latexfile(latexpath);
    if (latexfile.open(QIODevice::ReadOnly| QIODevice::Text)) {
        //QTextStream in(&latexfile);
        //QString latextext = in.readAll();
        latexstring = QString(latexfile.readAll());
        qInfo() << latexstring;
    }

    //ensure latexstring exists and does not contain ''' (python triple quotes)
    //can also get python to open the latex file! (rather than cpp)
    //put the following code into a resource python file prepending it with predefined variables from cpp (currentpath, latexstring, etc)
    std::vector<std::string> code = {
        "import sys\n",
        "sys.path.append('",QDir::currentPath().toStdString(),"\\\\python\\\\texsoup')\n",
        "print(sys.path)\n",
        "import TexSoup\n",
        "soup = TexSoup.TexSoup(r'''",latexstring.toStdString(),"''')\n",
        "print(soup.find_all('includegraphics')[0].args[1])"
    };

    std::stringstream codestream;
    std::string py_command_begin = "";
    std::string py_command_end = "";

    codestream << py_command_begin;
    for (std::string& codeline : code) {
        codestream << codeline;
    }
    codestream << py_command_end;
    std::cout << codestream.str() << std::endl;
    //    std::cout << QDir::currentPath().toStdString() << std::endl;
    std::string codestring = codestream.str();
    const char* codechar = codestring.c_str();


    Py_Initialize();
    PyRun_SimpleString(codechar);
    Py_Finalize();


}



