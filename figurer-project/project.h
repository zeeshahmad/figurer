#ifndef PROJECT_H
#define PROJECT_H

//holds and provides an interface to project information
//this is the core of project information

#include <QObject>
#include <QString>
#include <QJsonObject>

#include "externalfile.h"
#include "projectfileio.h"
#include "pythonthread.h"
#include "pythonuser.h"

class Project : public QObject, public PythonUser
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

    explicit Project(PythonThread* pt, ExistingFileParams& , QObject *parent = nullptr);
    explicit Project(PythonThread* pt, NewFileParams&, QObject *parent = nullptr);
    //noncopyable
    Project(const Project&)=delete;
    Project& operator=(const Project&)=delete;
    ~Project();

public Q_SLOTS:
    void consolidateFigureList(const QList<QString>& newFigList);
private:

    QString latexstring;
    QJsonObject jsonData;

    ProjectFileIO projectFileIO;
    ExternalFile* externalFile;

    void init();
    void makeConnections();
    void initJson();
};

#endif // PROJECT_H
