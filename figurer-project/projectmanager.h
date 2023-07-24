#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H
//manage the state of "open" project object(s)
//(i.e. store currently open project)
//triggered by ui to make and destroy project objects

//consider making this singleton
#include "project.h"
#include "pythonuser.h"
#include "pythonthread.h"
#include <QObject>

class ProjectManager : public QObject, public PythonUser
{
    Q_OBJECT
public:
    explicit ProjectManager(PythonThread *pythonThread, QObject *parent = nullptr);

    bool isAProjectOpen();

    Project* currentProject = nullptr;

Q_SIGNALS:
    void projectOpened();
    void projectClosed();

public Q_SLOTS:
    void createProjectRequested(QString& pathToNewFile, QString& pathToExternalFile);
    void openProjectRequested(QString& pathToOpenFile);
    void closeProjectRequested();

private:

};

#endif // PROJECTMANAGER_H
