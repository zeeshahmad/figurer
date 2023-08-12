#include "externalfile.h"
#include "iexternalfileparser.h"

#include <QDebug>
#include <memory>

ExternalFile::ExternalFile(PythonThread *pt, QString& filePath, QObject *parent)
    : QObject{parent}, PythonUser{pt}, filePath{filePath}, parserChooser{pt}
{
    assignParser(filePath);
}

void ExternalFile::assignParser(QString &pathWithExtension)
{
    if (parser == nullptr) {
        parser =std::unique_ptr<IExternalFileParser>( parserChooser.chooseUsingFileExtension(pathWithExtension) );
    } else {
        qFatal() << "Can't reassign a parser";
    }
}

void ExternalFile::rescan()
{
    QJsonObject updatedInfo = parser->parse(filePath);
    data = updatedInfo["includegraphics"].toVariant().toStringList();
    Q_EMIT rescanned(data);
}
