#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H
//manage the state of "open" project object(s)
//(i.e. store currently open project)
//triggered by ui to make and destroy project objects

#include "project.h"
#include "projecttools.h"
#include <QObject>

class ProjectManager : public QObject
{
    Q_OBJECT
public:
    ProjectManager(ProjectTools* tools, QObject *parent = nullptr);

    bool isAProjectOpen();


Q_SIGNALS:
    void projectOpened(QString );
    void projectClosed();

    void error_projectalreadyopen(QString);
    void error_noprojecttoclose(QString);


public Q_SLOTS:
    void createProjectRequested(QString& pathToNewFile, QString& pathToExternalFile);
    void openProjectRequested(QString& pathToOpenFile);
    void closeProjectRequested();


private:
    Project* currentProject = nullptr;
    ProjectTools* tools;

};

#endif // PROJECTMANAGER_H
