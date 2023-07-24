#ifndef EXTERNALFILEPARSERCHOOSER_H
#define EXTERNALFILEPARSERCHOOSER_H

#include <QString>

#include "iexternalfileparser.h"
#include "pythonuser.h"
#include "pythonthread.h"

class ExternalFileParserChooser: public PythonUser
{
public:
    ExternalFileParserChooser(PythonThread * pt);
    IExternalFileParser* chooseUsingFileExtension(QString& filePathWithExtension);
};

#endif // EXTERNALFILEPARSERCHOOSER_H
