#ifndef PYTHONTHREAD_H
#define PYTHONTHREAD_H

#include <QMutex>
#include <QThread>
#include <QList>
#include <QString>
#include <QSharedPointer>

class PythonThread : public QThread
{
    Q_OBJECT
public:
    struct Code {
        Code(QString content="", QString tag="",
             QObject* callingObject= nullptr,
             const char *callbackName = nullptr,
             const char *stringVarName = nullptr)
            :content{content}, tag{tag},
            callingObject{callingObject},
            callbackName{callbackName},
            stringVarName{stringVarName}
            {};
        QString content{""};
        QString tag{""};
        QObject* callingObject{nullptr};
        const char *callbackName{nullptr};
        const char *stringVarName{nullptr};
    };

    explicit PythonThread();

    void run();
    void queueCode(Code codeBlock);
    void cancelCodesWithTag(QString tag);
    QList<Code> pythonQueue;
    QMutex loopMutex;
    QMutex mutex;
Q_SIGNALS:
    void resultReady(QSharedPointer<QString>);
};

#endif // PYTHONTHREAD_H
