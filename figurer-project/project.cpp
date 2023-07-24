#include "project.h"

#include <QDebug>
#include <QFile>
#include <QDir>

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
    externalFileIO = new ExternalFile(pythonThread, externalFilePath);
}

Project::Project(PythonThread* pt, NewFileParams& params, QObject *parent)
    : QObject{parent}, PythonUser{pt},
    projectFileIO{params.projectFilePath, jsonData}
{
    jsonData.insert("externalFilePath", params.externalFilePath);
    projectFileIO.writeFile();
    externalFileIO = new ExternalFile(pythonThread, params.externalFilePath);
}

Project::~Project()
{
    delete externalFileIO;
}

