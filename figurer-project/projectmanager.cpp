#include "projectmanager.h"

#include <QDebug>

ProjectManager::ProjectManager(QObject *parent)
    : QObject{parent}
{

}

bool ProjectManager::isAProjectOpen()
{
    return (this->currentProject != nullptr);
}

void ProjectManager::createProjectRequested(QString& pathToNewFile, QString& pathToExternalFile)
{
    if (isAProjectOpen()){
        qWarning() << "Can't create a new project when another project is already open!";
    } else {
        Project::NewFileParams params;
        params.projectFilePath = pathToNewFile;
        params.externalFilePath = pathToExternalFile;
        currentProject = new Project(params);
        Q_EMIT projectOpened();
    }
}

void ProjectManager::openProjectRequested(QString& openedFilePath)
{
    if (isAProjectOpen()){
        qWarning() << "Can't open a project when another project is already open!";

    } else {
        Project::ExistingFileParams params;
        params.projectFilePath = openedFilePath;
        currentProject = new Project(params);
        Q_EMIT projectOpened();
    }
}

void ProjectManager::closeProjectRequested()
{
    if (isAProjectOpen()) {
        delete currentProject;
        currentProject = nullptr;
        Q_EMIT projectClosed();
    }
    else {
        qWarning() <<"No project is open to be able to close!";
    }
}
