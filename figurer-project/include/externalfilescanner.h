#ifndef EXTERNALFILESCANNER_H
#define EXTERNALFILESCANNER_H

#include "iexternalfileparser.h"

#include <QJsonObject>
#include <QObject>
#include <QList>

class ExternalFileScanner : public QObject
{
    Q_OBJECT
public:
    explicit ExternalFileScanner(QObject *parent = nullptr);
    ~ExternalFileScanner();
    QList<QString> scan(QString filePath);

Q_SIGNALS:
    void error_nosuitableparser(QString);

public Q_SLOTS:
    void addParser(IExternalFileParser *parser);
    void scanSlot(QString filePath, QList<QString>& varHandle);
private:
    QList<IExternalFileParser*> parsers;
    IExternalFileParser *chooseParser(QString filePath);
};

#endif // EXTERNALFILESCANNER_H
