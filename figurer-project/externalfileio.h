#ifndef EXTERNALFILEIO_H
#define EXTERNALFILEIO_H

#include "iexternalfileparser.h"
#include <QObject>

class ExternalFileIO : public QObject
{
    Q_OBJECT
public:
    explicit ExternalFileIO(IExternalFileParser& parser, QObject *parent = nullptr);

Q_SIGNALS:

private:
    IExternalFileParser& parser;

};

#endif // EXTERNALFILEIO_H
