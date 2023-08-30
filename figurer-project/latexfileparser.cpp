#include "latexfileparser.h"
#include "qjsondocument.h"

#include <QDebug>
#include <QString>
#include <QDir>

#include <string>


LatexFileParser::LatexFileParser(PythonThread *pt)
    :QObject{nullptr},PythonUser{pt}
{

}

QJsonObject LatexFileParser::parse(QString &filePath)
{
    QFile latexfile(filePath);
    QString latexstring;
    if (latexfile.open(QIODevice::ReadOnly| QIODevice::Text)) {
        latexstring = QString(latexfile.readAll());
        latexfile.close();
    } else qWarning()<<"Couldn't open latex file for parsing.";

    QFile pythonScanCodeFile(":/python/scan_latex.py");
    QString pythonScanCode;
    if (pythonScanCodeFile.open(QIODevice::ReadOnly|QIODevice::Text)) {
        pythonScanCode = QString::fromUtf8(pythonScanCodeFile.readAll());
        pythonScanCodeFile.close();
    } else qWarning()<<"Couldn't open python code used for scanning latex files.";


    PythonThread::Code code(pythonScanCode.arg(QDir::currentPath(),latexstring), "latexfileparser",
        this, "receiveScanResult", "scan_result");

    QFuture<QString> resultFuture = pythonThread->queueCode(code);
    resultFuture.waitForFinished();
    qInfo() << "future finished: " << resultFuture.result();
    QJsonDocument resultJson = QJsonDocument::fromJson(resultFuture.result().toUtf8());
    return resultJson.object();

}

bool LatexFileParser::canParse(QString &filePath)
{
    QFileInfo fileInfo(filePath);
    QString suffix = fileInfo.suffix();
    return (suffix.compare("tex", Qt::CaseSensitivity::CaseInsensitive) == 0);
}



