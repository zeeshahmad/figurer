#include "latexfileparser.h"
#include "qjsondocument.h"

#include <QDebug>
#include <QString>
#include <QDir>

#include <string>


LatexFileParser::LatexFileParser(pycode::Worker *pw)
    :QObject{nullptr},PythonUser{pw}
{

}

QJsonObject LatexFileParser::parse(const QString &filePath)
{
    QFile latexfile(filePath);
    QString latexstring;
    QJsonDocument resultJson;
    if (latexfile.open(QIODevice::ReadOnly| QIODevice::Text)) {
        latexstring = QString(latexfile.readAll());
        latexfile.close();
    } else {
        Q_EMIT error_latexfilecouldnotopen(QString("Couldn't open latex file %1 for parsing.").arg(filePath));
        return resultJson.object();
    }

    QFile pythonScanCodeFile(":/python/scan_latex.py");
    QString pythonScanCode;
    if (pythonScanCodeFile.open(QIODevice::ReadOnly|QIODevice::Text)) {
        pythonScanCode = QString::fromUtf8(pythonScanCodeFile.readAll());
        pythonScanCodeFile.close();
    } else {
        Q_EMIT error_couldnotopenpythoncode("Couldn't open python code used for scanning latex files.");
        return resultJson.object();
    }

    QString code = pythonScanCode.arg(QDir::currentPath(),latexstring);
    QString resultString = pythonWorker->enqueue(code, "scan_result").result().toString();
    //todo: checks for success of results are missing here!

    qInfo() << "Parsing python finished: " << resultString;
    resultJson = QJsonDocument::fromJson(resultString.toUtf8());

    if (resultJson.object().contains("error")) {
        QString latexerror = resultJson.object()["error"].toString();
        Q_EMIT error_latexerror(QString("There was a problem parsing the latex file: %1").arg(latexerror));
    }
    return resultJson.object();

}

bool LatexFileParser::canParse(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    QString suffix = fileInfo.suffix();
    return (suffix.compare("tex", Qt::CaseSensitivity::CaseInsensitive) == 0);
}

