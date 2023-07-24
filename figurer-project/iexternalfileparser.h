#ifndef IEXTERNALFILEPARSER_H
#define IEXTERNALFILEPARSER_H

#include <QString>

class IExternalFileParser
{
public:
    virtual ~IExternalFileParser() = default;
    virtual void parse(QString& filePath)=0;
};

#endif // IEXTERNALFILEPARSER_H
