#ifndef EXTERNALFILE_H
#define EXTERNALFILE_H

#include "externalfileparserchooser.h"
#include "iexternalfileparser.h"
#include "pythonuser.h"

#include <QJsonObject>
#include <QObject>
#include <QList>
#include <memory>


class ExternalFile : public QObject, public PythonUser
{
    Q_OBJECT
public:
    explicit ExternalFile(PythonThread* pt, QString& filePath, QObject *parent = nullptr);


    QList<QString> data;
Q_SIGNALS:
    void rescanned(const QList<QString>&);

public Q_SLOTS:
    void rescan();
private:
    std::unique_ptr<IExternalFileParser> parser;
    ExternalFileParserChooser parserChooser;
    void assignParser(QString& pathWithExtension);
    QString& filePath;
};

#endif // EXTERNALFILE_H
