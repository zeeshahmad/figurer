#ifndef PROJECTTOOLS_H
#define PROJECTTOOLS_H

#include "externalfilescanner.h"
#include "projectfileio.h"
#include <QObject>

class ProjectTools : public QObject
{
    Q_OBJECT
public:
    explicit ProjectTools(ProjectFileIO* io,
                          ExternalFileScanner* scanner,
                          QObject *parent = nullptr);

    ProjectFileIO *io;
    ExternalFileScanner *scanner;
Q_SIGNALS:

private:

};

#endif // PROJECTTOOLS_H
