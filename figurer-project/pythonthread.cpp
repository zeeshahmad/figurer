#include "pythonthread.h"
#include <QDebug>
#include <QList>
#include <QMutableListIterator>
#include <QMutexLocker>
#include <QSharedPointer>
#include <string>
#include <QFuture>
#include <QVariant>

#include "pybind11/embed.h"

namespace py=pybind11;

PythonThread::PythonThread()
    : QThread{nullptr}
{
    this->setObjectName("python thread");
}

PythonThread::~PythonThread()
{    
    if (isRunning()){
        requestInterruption();
        quit();
        wait();
    }
}

void PythonThread::run()
{
    py::scoped_interpreter guard{};
    qInfo() << "Starting python thread loop";
    while (!isInterruptionRequested()) {
        QMutexLocker loopLocker(&loopMutex);
        bool isThereCode = !pythonQueue.isEmpty();
        if (isThereCode) {
            PythonThread::Code* code = pythonQueue.takeFirst();
            const std::string rawCode = code->content.toStdString();
            try {
                loopLocker.unlock();
                py::exec(rawCode);
                loopLocker.relock();
                processCodeResult(code);
            } catch( std::exception &e) {
                qInfo()<<"std::exception whilst python execution: " << e.what();
                if (!loopLocker.isLocked()) loopLocker.relock();
                code->error = QString("std::exception whilst python execution: %1").arg(e.what());
            } catch (...) {
                qWarning() << "Unknown exception whilst python execution!";
            }
            if (!loopLocker.isLocked()) loopLocker.relock();
            code->promise->finish();
        }
    }
    qInfo() << "Ending python thread loop";
}

void PythonThread::processCodeResult(PythonThread::Code *code)
{
    const QString& pythonVarName = code->stringVarName;
    std::string typeEvalCode = QString("type(%1).__name__")
                                   .arg(pythonVarName).toStdString();
    QString varType(py::eval(typeEvalCode).cast<std::string>().c_str());

    if (varType == "int") {
        int var = py::eval(pythonVarName.toStdString()).cast<int>();
        code->promise->addResult(var);
    } else if (varType == "str") {
        QString var(py::eval(pythonVarName.toStdString()).cast<std::string>().c_str());
        code->promise->addResult(QString(var));
    } else {
        code->error = QString("Could not convert type %1 from python code").arg(varType);
    }
}

void PythonThread::addToQueue(PythonThread::Code& codeBlock, PythonThread::Watcher* watcherForConnection)
{
    codeBlock.promise = QSharedPointer<QPromise<QVariant>>::create();
    if (watcherForConnection != nullptr) watcherForConnection->setFuture(codeBlock.promise->future());
    codeBlock.promise->start();
    mutex.lock();
    qInfo() << QString("Queueing code with tag '%2' in position %1").arg(pythonQueue.count()).arg(codeBlock.tag);
    pythonQueue.append(&codeBlock);
    mutex.unlock();
}

void PythonThread::cancelCodesWithTag(const QString& tag)
{
    QMutexLocker locker(&mutex);
    QMutableListIterator<PythonThread::Code*> i(pythonQueue);
    while(i.hasNext()) {
        PythonThread::Code *code = i.next();
        if(code->tag.compare(tag, Qt::CaseSensitivity::CaseInsensitive)==0) {
            i.remove();
            code->error = QString("Cancelled");
            code->promise->finish();
        }
    }
}

const bool PythonThread::Code::successAfterWaiting()
{
    promise->future().waitForFinished();
    return error == "0";
}

const QVariant PythonThread::Code::result() const
{
    return promise->future().result();
}
