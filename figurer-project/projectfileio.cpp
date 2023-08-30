#include "projectfileio.h"

#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>


ProjectFileIO::ProjectFileIO(QObject* parent)
    :QObject{parent}
{

}

void ProjectFileIO::readFile(QString filePath, QJsonObject& dataHandle)
{
    QFile file;
    file.setFileName(filePath);
    QString contents;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        contents = file.readAll();
        file.close();
    }
    //could add a validation of json data using a schema
    dataHandle = QJsonDocument::fromJson(contents.toUtf8()).object();
}


void ProjectFileIO::writeFile(QString filePath, QJsonObject& dataHandle)
{
    QJsonDocument document;
    //again could add validation of data here
    document.setObject(dataHandle);
    QByteArray bytes = document.toJson(QJsonDocument::Indented);
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream iStream(&file);
        iStream.setEncoding(QStringConverter::Utf8);
        iStream << bytes;
        file.close();
    }
}

