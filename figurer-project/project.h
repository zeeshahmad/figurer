#ifndef PROJECT_H
#define PROJECT_H

//instantiates and holds project information
//this is the core of project information

#include <QObject>
#include <QString>
#include <QJsonObject>

#include "externalfileio.h"
#include "externalfileparserchooser.h"
#include "projectfileio.h"

class Project : public QObject
{
    Q_OBJECT
public:
    struct NewFileParams {
        QString projectFilePath;
        QString externalFilePath;
    };

    struct ExistingFileParams {
        QString projectFilePath;
    };


    explicit Project(ExistingFileParams& , QObject *parent = nullptr);
    explicit Project(NewFileParams&, QObject *parent = nullptr);
    ~Project();

Q_SIGNALS:

private:

    QString latexstring;
    QJsonObject jsonData;

    ProjectFileIO projectFileIO;
    ExternalFileIO* externalFileIO;
    IExternalFileParser* externalParser = nullptr;
    ExternalFileParserChooser parserChooser;
    void assignParser(QString& pathWithExtension);

};

#endif // PROJECT_H
