#ifndef IEXTERNALFILEPARSER_H
#define IEXTERNALFILEPARSER_H


class IExternalFileParser
{
public:
    virtual ~IExternalFileParser() = default;
    virtual void parse()=0;
};

#endif // IEXTERNALFILEPARSER_H
