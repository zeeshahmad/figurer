#ifndef PROJECTFILEIO_H
#define PROJECTFILEIO_H

//this object is the link between disk and Project object triggered by ui, and thus manages saving and reading

#include <QObject>
#include <QString>
#include <QJsonObject>

class ProjectFileIO: public QObject
{
    Q_OBJECT
public:
    ProjectFileIO(QString& projectFilePath, QJsonObject& projectData);
//    void newFile();
    void readFile();
    void writeFile();

private:
    QJsonObject& data;
    QString& filePath;


Q_SIGNALS:
};

#endif // PROJECTFILEIO_H
