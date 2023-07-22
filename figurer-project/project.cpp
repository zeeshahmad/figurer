#include "project.h"

#include <QDebug>
#include <QFile>
#include <QDir>

#include <string>
#include <iostream>
#include <stdio.h>

#include <sstream>
#include <vector>

#include <pybind11/embed.h>


namespace py = pybind11;


Project::Project(ExistingFileParams& params, QObject *parent)
    : QObject{parent}, projectFileIO{params.projectFilePath, jsonData}
{
    projectFileIO.readFile();
    QString externalFilePath = jsonData["externalFilePath"].toString();
    qInfo() << "in Project constructor(existing) : " << externalFilePath;
    assignParser(externalFilePath);
    externalFileIO = new ExternalFileIO(*externalParser);
}

Project::Project(NewFileParams& params, QObject *parent)
    : QObject{parent}, projectFileIO{params.projectFilePath, jsonData}
{
    jsonData.insert("externalFilePath", params.externalFilePath);
    projectFileIO.writeFile();
    assignParser(params.externalFilePath);
    externalFileIO = new ExternalFileIO(*externalParser);
}

Project::~Project()
{
    delete externalFileIO;
    delete externalParser;
}

void Project::assignParser(QString &pathWithExtension)
{
    if (externalParser == nullptr) {
        externalParser = parserChooser.chooseUsingFileExtension(pathWithExtension);
    } else {
        qFatal() << "Can't reassign a parser";
    }
}
