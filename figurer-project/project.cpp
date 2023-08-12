#include "project.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>

#include <string>
#include <iostream>
#include <stdio.h>

#include <sstream>
#include <vector>

Project::Project(PythonThread* pt, ExistingFileParams& params, QObject *parent)
    : QObject{parent}, PythonUser{pt},
    projectFileIO{params.projectFilePath, jsonData}
{
    projectFileIO.readFile();
    QString externalFilePath = jsonData["externalFilePath"].toString();
    qInfo() << "in Project constructor(existing) : " << externalFilePath;
    externalFile = new ExternalFile(pythonThread, externalFilePath);
    init();
}

Project::Project(PythonThread* pt, NewFileParams& params, QObject *parent)
    : QObject{parent}, PythonUser{pt},
    projectFileIO{params.projectFilePath, jsonData}
{
    jsonData.insert("externalFilePath", params.externalFilePath);
    projectFileIO.writeFile();
    externalFile = new ExternalFile(pythonThread, params.externalFilePath);
    init();
}

void Project::init()
{
    initJson();
    makeConnections();
    externalFile->rescan();
}

void Project::makeConnections()
{
    connect(externalFile, SIGNAL(rescanned(QList<QString>)), this, SLOT(consolidateFigureList(QList<QString>)));
}

void Project::initJson()
{
    if (!jsonData["figures"].isArray())
        jsonData["figures"] = QJsonArray();
}

Project::~Project()
{
    delete externalFile;
}

void Project::consolidateFigureList(const QList<QString>& updatedFigList)
{
    QStringList newList = QStringList(updatedFigList);
    const QJsonArray& figures = jsonData["figures"].toArray();
    QStringList danglingIds;
    for (const QJsonValue& val : figures) {//looping through existing figures
        const QJsonObject& existingFig = val.toObject();
        const QString id = existingFig["id"].toString();
        if (newList.contains(id)){
            newList.removeOne(id);
        } else {
            danglingIds.push_back(id);
        }
    }

    if (danglingIds.length()>0) {
        qInfo() << "Dangling figure ids in project: "<< danglingIds;
    }

    QStringListIterator newListIt(newList);
    QJsonArray newFiguresJson = jsonData["figures"].toArray();
    while(newListIt.hasNext()) {
        QString newFigId = newListIt.next();
        qInfo() << "Discovered new fig id: " << newFigId << ", adding to project.";
        QJsonObject newFig;
        newFig["id"] = newFigId;
        newFiguresJson.append(newFig);
    }
    jsonData["figures"] = newFiguresJson;
    projectFileIO.writeFile();
}





