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

    QJsonObject parse(const QString& filePath);

    bool canParse(const QString &filePath);

Q_SIGNALS:
    void error_latexfilecouldnotopen(QString);
    void error_couldnotopenpythoncode(QString);
    void error_latexerror(QString);


public Q_SLOTS:
};

#endif // LATEXFILEPARSER_H
