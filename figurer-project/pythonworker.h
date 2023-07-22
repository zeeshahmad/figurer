#ifndef PYTHONWORKER_H
#define PYTHONWORKER_H

#include <QObject>
#include <QString>
#include <QThread>
#include <QMutex>

#include <pybind11/embed.h>

namespace py = pybind11;

class PythonWorker : public QObject
{
    Q_OBJECT
public:
    explicit PythonWorker(QObject *parent = nullptr);

    ~PythonWorker();

    QThread *pythonThread;
    py::dict* pythonLocals;

public Q_SLOTS:
    void runPython(QString *);
    void startPython();
    void endPython();

Q_SIGNALS:

private:
    QMutex mutex;
};

#endif // PYTHONWORKER_H
