#ifndef LATEXFILEPARSER_H
#define LATEXFILEPARSER_H

//this object is for extracting info needed from given latex file

//in case i need to inherit qobject :
//https://forum.qt.io/topic/131892/interfaces-must-inherit-qobject

#include "iexternalfileparser.h"
#include "pythonuser.h"


class LatexFileParser: public IExternalFileParser, public PythonUser
{
public:
    LatexFileParser(PythonThread*pt);

    void parse(QString& filePath);
};

#endif // LATEXFILEPARSER_H