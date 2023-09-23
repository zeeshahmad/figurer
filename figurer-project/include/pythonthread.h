#ifndef PYTHONTHREAD_H
#define PYTHONTHREAD_H

#include <QMutex>
#include <QThread>
#include <QList>
#include <QString>
#include <QSharedPointer>
#include <QFuture>
#include <QFutureWatcher>
#include <QVariant>


class PythonThread : public QThread
{
    Q_OBJECT;

public:
    using Watcher = QFutureWatcher<QVariant>;
    struct Code {
        Code(QString content, QString stringVarName, QString tag="")
            :content{content}, stringVarName{stringVarName}, tag{tag}
            {};
        QString content;
        QString stringVarName;
        QString tag{""};
        QSharedPointer<QPromise<QVariant>> promise;
        QString error = "0";
        const bool successAfterWaiting();
        const QVariant result() const;
    };


    explicit PythonThread();
    ~PythonThread();

    void addToQueue(Code& codeBlock, PythonThread::Watcher *watcherForConnection = nullptr);
    void cancelCodesWithTag(const QString &tag);
private:
    QMutex mutex;
    QMutex loopMutex;
    QList<Code*> pythonQueue;

    void run();
    void processCodeResult(Code *code);
};

#endif // PYTHONTHREAD_H
