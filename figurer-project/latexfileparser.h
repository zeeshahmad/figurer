#ifndef LATEXFILEPARSER_H
#define LATEXFILEPARSER_H

//this object is for extracting info needed from given latex file

//in case i need to inherit qobject :
//https://forum.qt.io/topic/131892/interfaces-must-inherit-qobject

#include "iexternalfileparser.h"
#include "pythonuser.h"
#include <QObject>

class LatexFileParser: public QObject, public IExternalFileParser, public PythonUser
{
    Q_OBJECT
public:
    LatexFileParser(PythonThread*pt);

    void parse(QString& filePath);

public Q_SLOTS:
    void recievePythonResult(QSharedPointer<QString> result);
};

#endif // LATEXFILEPARSER_H
