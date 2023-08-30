#include "externalfilescanner.h"
#include "iexternalfileparser.h"

#include <QFileInfo>
#include <QDebug>

ExternalFileScanner::ExternalFileScanner(QObject *parent)
    : QObject{parent}
{

}

ExternalFileScanner::~ExternalFileScanner()
{

}


QList<QString> ExternalFileScanner::scan(QString filePath)
{
//    assignParser(filePath);
    IExternalFileParser* parser = chooseParser(filePath);
    QJsonObject updatedInfo = parser->parse(filePath);
    QList<QString> data = updatedInfo["includegraphics"].toVariant().toStringList();
    return data;
}

void ExternalFileScanner::addParser(IExternalFileParser *parser)
{
    parsers.append(parser);
}

void ExternalFileScanner::scanSlot(QString filePath, QList<QString> &varHandle)
{
    varHandle = scan(filePath);
}

IExternalFileParser *ExternalFileScanner::chooseParser(QString filePath)
{
    IExternalFileParser* chosen = nullptr;
    QListIterator<IExternalFileParser*> it(parsers);
    while(it.hasNext()) {
        IExternalFileParser* currentParser = it.next();
        if (currentParser->canParse(filePath)){
            chosen = currentParser;
            break;
        }
    }
    if (chosen == nullptr) {
        qInfo() << "ExternalFileScanner could not choose a suitable parser for file: "
                << filePath;
    }
    return chosen;
}
