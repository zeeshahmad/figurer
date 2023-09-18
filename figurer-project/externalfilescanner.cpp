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
    QStringList data;
    IExternalFileParser* parser = chooseParser(filePath);
    if (parser) {
        QJsonObject updatedInfo = parser->parse(filePath);
        data = updatedInfo["includegraphics"].toVariant().toStringList();
    }
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

        Q_EMIT error_nosuitableparser(QString("Could not choose a suitable parser for file: %1").arg(filePath));
    }
    return chosen;
}
