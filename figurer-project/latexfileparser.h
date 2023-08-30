#ifndef LATEXFILEPARSER_H
#define LATEXFILEPARSER_H

//this object is for extracting info needed from given latex file


#include "iexternalfileparser.h"
#include "pythonuser.h"
#include <QObject>

class LatexFileParser: public QObject, public IExternalFileParser, public PythonUser
{
    Q_OBJECT
public:
    LatexFileParser(PythonThread*pt);

    QJsonObject parse(QString& filePath);

    bool canParse(QString& filePath);

public Q_SLOTS:
};

#endif // LATEXFILEPARSER_H
