#ifndef IEXTERNALFILEPARSER_H
#define IEXTERNALFILEPARSER_H

#include <QString>
#include <QJsonObject>

class IExternalFileParser
{
public:
    virtual ~IExternalFileParser() = default;
    virtual QJsonObject parse(const QString& filePath)=0;
    virtual bool canParse(const QString& filePath) =0;


};

#endif // IEXTERNALFILEPARSER_H
