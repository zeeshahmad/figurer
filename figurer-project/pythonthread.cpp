#include "pythonthread.h"
#include <QDebug>
#include <QList>
#include <QMutexLocker>

#include "pybind11/embed.h"

namespace py=pybind11;

PythonThread::PythonThread()
    : QThread{nullptr}
{

}

void PythonThread::run()
{
    py::scoped_interpreter guard{};
    qInfo() << "started to run";

    while (!isInterruptionRequested()) {
        loopMutex.lock();
        bool isThereCode = !pythonQueue.isEmpty();
        loopMutex.unlock();
        if (isThereCode) {
            loopMutex.lock();
            QString nextBlockOfCode(pythonQueue.takeFirst());
            loopMutex.unlock();
            try {
                py::exec(nextBlockOfCode.toStdString());
                qInfo()<< py::eval("a").cast<int>();
            } catch( std::exception &e) {
                qWarning()<<"some shit happend with python: " << e.what();
            } catch (...) {
                qWarning() << "Some bizzare shit happened when trying to run python code";
            }
        }

    }

}

void PythonThread::appendToQueue(QString codeBlock)
{
    loopMutex.lock();
    pythonQueue.append(codeBlock);
    loopMutex.unlock();
}
