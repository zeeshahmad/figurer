#include "externalfileparserchooser.h"

#include <QString>
#include <QFileInfo>
#include <QDebug>

#include "iexternalfileparser.h"
#include "latexfileparser.h"

IExternalFileParser* ExternalFileParserChooser::chooseUsingFileExtension(QString &filePathWithExtension)
{
    QFileInfo fileInfo(filePathWithExtension);
    QString suffix = fileInfo.suffix();

    IExternalFileParser* newparser = nullptr;
    qInfo() << suffix;
    if (suffix.compare("tex", Qt::CaseSensitivity::CaseInsensitive) == 0){
        newparser = new LatexFileParser();
    }

    if (newparser==nullptr) qFatal()<< "Unable to choose a parser implementation!";

    return newparser;
}
