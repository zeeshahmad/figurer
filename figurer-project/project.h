#ifndef PROJECT_H
#define PROJECT_H

//provides correct format for data into the project file in json format

#include "projecttools.h"
#include <QObject>
#include <QString>
#include <QJsonObject>


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

    const QString projectFilePath;


    explicit Project(ProjectTools* tools, ExistingFileParams& , QObject *parent = nullptr);
    explicit Project(ProjectTools* tools, NewFileParams&, QObject *parent = nullptr);
    //noncopyable
    Project(const Project&)=delete;
    Project& operator=(const Project&)=delete;
    ~Project();

    QString getInfo(const QString& infoKey);

Q_SIGNALS:
    void foundDanglingIds(QStringList ids);
public Q_SLOTS:
    void consolidateFigureList(const QList<QString>& newFigList);
private:
    void init();
    ProjectTools* tools;

    QString latexstring;
    QJsonObject jsonData;

    void makeConnections();
    void initJson();
};

#endif // PROJECT_H
