#include "project.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>


Project::Project(ProjectTools *tools, ExistingFileParams& params, QObject *parent)
    : QObject{parent}, projectFilePath{params.projectFilePath},tools{tools}
{
    tools->io->readFile(params.projectFilePath, jsonData);
    init();
}

Project::Project(ProjectTools *tools, NewFileParams& params, QObject *parent)
    : QObject{parent}, projectFilePath{params.projectFilePath},tools{tools}
{
    jsonData.insert("externalFilePath", params.externalFilePath);
    init();
    tools->io->writeFile(params.projectFilePath, jsonData);
}


void Project::init()
{
    QStringList scannedFigList = tools->scanner->scan(getInfo("externalFilePath"));
    consolidateFigureList(scannedFigList);
    initJson();
}


void Project::initJson()
{
    if (!jsonData["figures"].isArray())
        jsonData["figures"] = QJsonArray();
}

Project::~Project()
{
}

QString Project::getInfo(const QString& infoKey)
{
    if (jsonData[infoKey].isString())
        return QString(jsonData[infoKey].toString());
    else return QString();
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
        Q_EMIT foundDanglingIds(danglingIds);
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
    tools->io->writeFile(projectFilePath, jsonData);
}





