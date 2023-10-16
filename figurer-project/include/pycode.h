#ifndef PYCODE_FIGURER_H
#define PYCODE_FIGURER_H

#include <QMutex>
#include <QThread>
#include <QList>
#include <QString>
#include <QSharedPointer>
#include <QFuture>
#include <QVariant>

namespace pycode {

    class Result{
    public:
        QVariant data;
        Result(){}
        Result(const QVariant& data): data(data) {}
        Result(const int& data): data(data) {}
        Result(const QString& data): data(data) {}
        const int toInt() const { return data.toInt(); }
        const QString toString() const { return data.toString(); }

        const bool success() const { return error_=="0"; }
        const QString error() const { return error_; }
        void setError(const QString& err) { error_ = err; }
    private:
        QString error_;
    };


    class Worker : public QThread
    {
        Q_OBJECT;

    public:

        explicit Worker();
        ~Worker();

        QFuture<Result> enqueue(const QString& pythonCode, const QString& stringVarName, const QString& tag= "tag");
        void cancelCodesWithTag(const QString &tag);
    private:
        struct Job {
            Job(QString content, QString stringVarName, QString tag="")
                :content{content}, stringVarName{stringVarName}, tag{tag} {}
            QString content;
            QString stringVarName;
            QString tag{""};
            QPromise<Result> promise;
        };
        QList<QSharedPointer<Job>> queue;
        QMutex mutex;
        QMutex loopMutex;

        void run();

        void processCodeResult(QSharedPointer<Job> code);
        void addJobToQueue(QSharedPointer<Job> job);
        void addErrorToPromise(const QString& errortext, QPromise<Result>* promise);
        void addSuccessToPromise(const QVariant& var, QPromise<Result>* promise);
    };
}
#endif // PYCODE_FIGURER_H
