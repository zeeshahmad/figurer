#include "project.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>


Project::Project(ProjectTools *tools, ExistingFileParams& params, pycode::Worker* pw,  QObject *parent)
    : QObject{parent}, projectFilePath{params.projectFilePath},tools{tools}, PythonUser{pw}
{
    restore();
}

Project::Project(ProjectTools *tools, NewFileParams& params, pycode::Worker* pw, QObject *parent)
    : QObject{parent}, projectFilePath{params.projectFilePath},tools{tools}, PythonUser{pw}
{

    init(params.externalFilePath);
    save();
}


void Project::init(QString externalFilePath)
{
    metadata["externalFilePath"] = externalFilePath;
}


Project::~Project()
{
}

void Project::restore()
{

    QString pythonCode = tools->io->readFile(projectFilePath);
    pythonCode += "\nimport inspect\n";
    pythonCode += "functions = {name: inspect.getsource(obj) for name, obj in globals().items() if callable(obj) and not name.startswith('_')}\n";
    pythonFunctions.clear();

    QJsonDocument doc = pythonWorker->enqueue(pythonCode, "functions").result().toJsonDocument();
    QJsonObject obj = doc.object();
    for (QJsonObject::const_iterator it= obj.begin(); it!=obj.end(); ++it) {
        QString name = it.key();
        QJsonValue value = it.value();
        pythonFunctions.insert(name, value.toString());
    }

    pythonCode = tools->io->readFile(projectFilePath);
    doc = pythonWorker->enqueue(pythonCode, "metadata").result().toJsonDocument();
    metadata = doc.object();
}

void Project::save()
{
    QJsonDocument doc(metadata);
    QString metadata_str = doc.toJson(QJsonDocument::Compact);
    QString pythonCode;
    pythonCode = "import json\n";
    pythonCode += QString("metadata = json.loads(R\"%1\")\n").arg(metadata_str);
    for (auto it=pythonFunctions.begin(); it!= pythonFunctions.end(); ++it) {
        pythonCode += it.value();
    }
    tools->io->writeFile(projectFilePath, pythonCode);
}

// TODO refactor this function to new design
// instead of this, make rescan external slot that scans for figures
// not in the pythonFunctions, and maintains a list of keys of pythonFunctions
// that is in the latex
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





