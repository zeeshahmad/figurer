#include "projectmanager.h"
#include "projecttools.h"

#include <QDebug>

ProjectManager::ProjectManager(ProjectTools* tools, QObject *parent)
    : QObject{parent},tools{tools}
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
        currentProject = new Project(tools, params);
        Q_EMIT projectOpened(pathToExternalFile);
    }
}

void ProjectManager::openProjectRequested(QString& openedFilePath)
{
    if (isAProjectOpen()){
        qWarning() << "Can't open a project when another project is already open!";

    } else {
        Project::ExistingFileParams params;
        params.projectFilePath = openedFilePath;
        currentProject = new Project(tools, params);
        Q_EMIT projectOpened(currentProject->getInfo("externalFilePath"));
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
