#include "projectfileio.h"

#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>


ProjectFileIO::ProjectFileIO(QString& projectFilePath, QJsonObject& projectData)
    :QObject{nullptr}, data{projectData}, filePath{projectFilePath}
{

}


void ProjectFileIO::readFile()
{
    QFile file;
    file.setFileName(filePath);
    QString contents;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        contents = file.readAll();
        file.close();
    }
    qInfo() << "In projectfileio readFile: " << contents;
    //could add a validation of json data using a schema
    data = QJsonDocument::fromJson(contents.toUtf8()).object();
}


void ProjectFileIO::writeFile()
{
    QJsonDocument document;
    //again could add validation of data here
    document.setObject(data);
    QByteArray bytes = document.toJson(QJsonDocument::Indented);
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream iStream(&file);
        iStream.setEncoding(QStringConverter::Utf8);
        iStream << bytes;
        file.close();
    }
}

