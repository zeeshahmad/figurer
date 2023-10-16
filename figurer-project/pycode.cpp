#include "pycode.h"
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

namespace pycode {

    Worker::Worker(): QThread{nullptr}
    {
    this->setObjectName("python thread");
    }

    Worker::~Worker()
    {
        if (isRunning()){
            requestInterruption();
            quit();
            wait();
        }
    }

    void Worker::run()
    {
        py::scoped_interpreter guard{};
        qInfo() << "Starting python thread loop";
        while (!isInterruptionRequested()) {
            QMutexLocker loopLocker(&loopMutex);
            bool isThereCode = !queue.isEmpty();
            if (isThereCode) {
                QSharedPointer<Job> job = queue.takeFirst();

                const std::string rawCode = job->content.toStdString();
                try {
                    loopLocker.unlock();
                    py::exec(rawCode);
                    loopLocker.relock();
                    processCodeResult(job);
                } catch( std::exception &e) {
                    qInfo()<<"std::exception whilst python execution: " << e.what();
                    if (!loopLocker.isLocked()) loopLocker.relock();
                    QString err = QString("std::exception whilst python execution: %1").arg(e.what());
                    addErrorToPromise(err, &job->promise);
                } catch (...) {
                    qWarning() << "Unknown exception whilst python execution!";
                }
                if (!loopLocker.isLocked()) loopLocker.relock();
                job->promise.finish();
            }
        }
        qInfo() << "Ending python thread loop";
    }

    QFuture<Result> Worker::enqueue(const QString &pythonCode, const QString &stringVarName, const QString & tag)
    {
        QSharedPointer<Job> job = QSharedPointer<Job>::create
            (pythonCode, stringVarName, tag);
        addJobToQueue(job);
        return job->promise.future();
    }

    void Worker::cancelCodesWithTag(const QString& tag)
    {
        QMutexLocker locker(&mutex);
        QMutableListIterator<QSharedPointer<Job>> i(queue);
        while(i.hasNext()) {
            QSharedPointer<Job> job = i.next();
            if(job->tag.compare(tag, Qt::CaseSensitivity::CaseInsensitive)==0) {
                i.remove();
                addErrorToPromise("Cancelled", &job->promise);
                job->promise.finish();
            }
        }
    }

    void Worker::processCodeResult(QSharedPointer<Job> job)
    {
        const QString& pythonVarName = job->stringVarName;
        std::string typeEvalCode = QString("type(%1).__name__")
                                       .arg(pythonVarName).toStdString();
        QString varType(py::eval(typeEvalCode).cast<std::string>().c_str());

        if (varType == "int") {
            int var = py::eval(pythonVarName.toStdString()).cast<int>();
            addSuccessToPromise(var, &job->promise);
        } else if (varType == "str") {
            QString var(py::eval(pythonVarName.toStdString()).cast<std::string>().c_str());
            addSuccessToPromise(var, &job->promise);
        } else {
            QString err = QString("Could not convert type %1 from python code").arg(varType);
            addErrorToPromise(err, &job->promise);
        }
    }

    void Worker::addJobToQueue(QSharedPointer<Job> job)
    {
        job->promise.start();
        mutex.lock();
        qInfo() << QString("Queueing code with tag '%2' in position %1").arg(queue.count()).arg(job->tag);
        queue.append(job);
        mutex.unlock();
    }

    void Worker::addErrorToPromise(const QString &errortext, QPromise<Result> *promise)
    {
        Result errorResult;
        errorResult.setError(errortext);
        promise->addResult(errorResult);
    }

    void Worker::addSuccessToPromise(const QVariant &variable, QPromise<Result> *promise)
    {
        Result successResult(variable);
        successResult.setError("0");
        promise->addResult(successResult);
    }

}
