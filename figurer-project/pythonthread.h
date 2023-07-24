#ifndef PYTHONTHREAD_H
#define PYTHONTHREAD_H

#include <QMutex>
#include <QThread>
#include <QList>
#include <QString>
class PythonThread : public QThread
{
public:
    explicit PythonThread();

    void run();
    void appendToQueue(QString codeBlock);
    QList<QString> pythonQueue;
    QMutex loopMutex;
    QMutex appendMutex;
};

#endif // PYTHONTHREAD_H
