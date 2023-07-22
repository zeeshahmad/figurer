#ifndef EXTERNALFILEPARSERCHOOSER_H
#define EXTERNALFILEPARSERCHOOSER_H

#include <QString>

#include "iexternalfileparser.h"

class ExternalFileParserChooser
{
public:
    IExternalFileParser* chooseUsingFileExtension(QString& filePathWithExtension);
};

#endif // EXTERNALFILEPARSERCHOOSER_H
