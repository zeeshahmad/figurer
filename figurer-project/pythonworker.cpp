#include "pythonworker.h"
#include "pybind11/pytypes.h"
#include <pybind11/embed.h>

#include <QDebug>
#include <QThread>
#include <QDir>

#include <iostream>
#include <string>

namespace py = pybind11;
using namespace py::literals;

PythonWorker::PythonWorker(QObject *parent)
    : QObject{parent}
{
    this->setObjectName("Python Worker");
    pythonThread = new QThread;
    pythonThread->setObjectName("Python Thread");

    connect(pythonThread, SIGNAL(started()), this, SLOT(startPython()));

    moveToThread(pythonThread);
    pythonThread->start();
    qInfo() << QThread::currentThread();
}

PythonWorker::~PythonWorker()
{
    this->endPython();
    pythonThread->quit();
    pythonThread->deleteLater();
}

void PythonWorker::runPython(QString * code)
{
    py::gil_scoped_release release;
    mutex.lock();
    qInfo() << "python worker " << this << " run on " << QThread::currentThread();
    qInfo() << *code;
    mutex.unlock();
    py::gil_scoped_acquire acquire;
    py::exec(R"(
        testcount += 1
    )", py::globals(), *pythonLocals);

    auto testcount = (*pythonLocals)["testcount"].cast<int>();
    qInfo() << testcount;
}

void PythonWorker::startPython()
{
    qInfo() << "Initializing interpreter on: " << QThread::currentThread();
    py::initialize_interpreter();
    pythonLocals = new py::dict();
    py::exec(R"(
        testcount = 0
    )", py::globals(), *pythonLocals);

}

void PythonWorker::endPython()
{
    py::finalize_interpreter();
}
