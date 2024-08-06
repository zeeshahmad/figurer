#ifndef PROJECT_H
#define PROJECT_H

//provides correct format for data into the project file in json format

#include "projecttools.h"
#include "pythonuser.h"
#include <QObject>
#include <QString>
#include <QHash>


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

    QString projectFilePath;
    const QString externalFilePath() const { return metadata["externalFilePath"].toString(); }


    explicit Project(ProjectTools* tools, ExistingFileParams& , pycode::Worker*, QObject *parent = nullptr);
    explicit Project(ProjectTools* tools, NewFileParams&, pycode::Worker* pw, QObject *parent = nullptr);
    //noncopyable
    Project(const Project&)=delete;
    Project& operator=(const Project&)=delete;
    ~Project();

    void restore();
    void save();


    QHash<QString, QString> pythonFunctions;
    QJsonObject metadata;


Q_SIGNALS:
    void foundDanglingIds(QStringList ids);
public Q_SLOTS:
    void consolidateFigureList(const QList<QString>& newFigList);
private:
    void init(QString externalFilePath);
    ProjectTools* tools;


};

#endif // PROJECT_H
