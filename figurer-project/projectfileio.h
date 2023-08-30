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
    ProjectFileIO(QObject* parent = nullptr);
public Q_SLOTS:
    virtual void readFile(QString filePath, QJsonObject& dataHandle);
    virtual void writeFile(QString filePath, QJsonObject& dataHandle);


Q_SIGNALS:
private:
};

#endif // PROJECTFILEIO_H
