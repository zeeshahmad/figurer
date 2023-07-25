#include "externalfile.h"
#include "iexternalfileparser.h"

#include <QDebug>
#include <memory>

ExternalFile::ExternalFile(PythonThread *pt, QString& filePath, QObject *parent)
    : QObject{parent}, PythonUser{pt}, filePath{filePath}, parserChooser{pt}
{
    assignParser(filePath);
    rescanFile();
}

void ExternalFile::assignParser(QString &pathWithExtension)
{
    if (parser == nullptr) {
        parser =std::unique_ptr<IExternalFileParser>( parserChooser.chooseUsingFileExtension(pathWithExtension) );
    } else {
        qFatal() << "Can't reassign a parser";
    }
}

void ExternalFile::rescanFile()
{
    parser->parse(filePath);
}
