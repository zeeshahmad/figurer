#include "projectfileio.h"

#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QTextStream>

ProjectFileIO::ProjectFileIO(QObject* parent)
    :QObject{parent}
{

}

QString ProjectFileIO::readFile(QString filePath)
{
    QFile file;
    file.setFileName(filePath);
    QString contents;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        contents = file.readAll();
        file.close();
        qInfo() << "Read file: "<< filePath;
    }
    return QString(contents);
}


void ProjectFileIO::writeFile(QString filePath, const QString &content)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream iStream(&file);
        iStream.setEncoding(QStringConverter::Utf8);
        iStream << content;
        file.close();
        qInfo() << "Write file: "<< filePath;
    }
}

