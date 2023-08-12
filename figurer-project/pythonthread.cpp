#include "pythonthread.h"
#include <QDebug>
#include <QList>
#include <QMutableListIterator>
#include <QMutexLocker>
#include <QSharedPointer>
#include <string>
#include <QFuture>

#include "pybind11/embed.h"

namespace py=pybind11;

PythonThread::PythonThread()
    : QThread{nullptr}
{
    this->setObjectName("python thread");
}

void PythonThread::run()
{
    py::scoped_interpreter guard{};
    qInfo() << "Starting python thread loop";
//    auto pylocals = py::dict();
    while (!isInterruptionRequested()) {
        QMutexLocker loopLocker(&loopMutex);
        bool isThereCode = !pythonQueue.isEmpty();
        if (isThereCode) {
            PythonThread::Code nextBlockOfCode = pythonQueue.takeFirst();
            nextBlockOfCode.promise->start();
            loopLocker.unlock();
            try {
                py::exec(nextBlockOfCode.content.toStdString());
                loopLocker.relock();
                std::string evalResult_std(py::eval(nextBlockOfCode.stringVarName).cast<std::string>());
                QSharedPointer<QString> evalResult = QSharedPointer<QString>::create(evalResult_std.c_str());
                if (nextBlockOfCode.callingObject) {
                    if (nextBlockOfCode.callbackName == nullptr) qFatal() << "No callback specified but calling object is not null.";
                    if (nextBlockOfCode.stringVarName == nullptr) qFatal() << "No callback python string var specified but calling object is not null.";
                    nextBlockOfCode.promise->addResult(QString(evalResult_std.c_str()));
//                    QMetaObject::invokeMethod(nextBlockOfCode.callingObject, nextBlockOfCode.callbackName,
//                          Qt::QueuedConnection, evalResult);
                }
            } catch( std::exception &e) {
                qWarning()<<"some shit happend during python: " << e.what();
//                qWarning() << "this is the code:"<<nextBlockOfCode.content;
            } catch (...) {
                qWarning() << "Some bizzare shit happened when trying to run python code";
            }
            if (!loopLocker.isLocked()) loopLocker.relock();
            nextBlockOfCode.promise->finish();
        }
    }
}

QFuture<QString> PythonThread::queueCode(PythonThread::Code& codeBlock)
{
    QSharedPointer<QPromise<QString>> promise = QSharedPointer<QPromise<QString>>::create();
    qInfo() << "Queueing code";
    codeBlock.promise = promise;
    mutex.lock();
    pythonQueue.append(codeBlock);
    mutex.unlock();
    return promise->future();
}

void PythonThread::cancelCodesWithTag(QString tag)
{
    QMutexLocker locker(&mutex);
    QMutableListIterator<PythonThread::Code> i(pythonQueue);
    while(i.hasNext()) {
        if(i.next().tag.compare(tag, Qt::CaseSensitivity::CaseInsensitive)==0) {
            i.remove();
        }
    }
}
