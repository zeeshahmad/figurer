#include <QTest>
#include <QSignalSpy>
#include <QTemporaryFile>
#include <QJsonDocument>
#include <QJsonArray>

#include "projectfileio.h"


class ProjectFileIOTest: public QObject
{
    Q_OBJECT;

private Q_SLOTS:
    void readFileSlotReadsValidJsonToHandle();
    void writeFileSlotWritesValidJsonToFile();
private:

};


void ProjectFileIOTest::readFileSlotReadsValidJsonToHandle()
{
    QTemporaryFile fileToRead("figurer-projectfileio-readfile.json");
    QString fileContent = QString(R"(
    {"string":"string", "arrayofobjs":[{"name":"obj1"},{"name":"obj2"}]}
    )");
    if (!fileToRead.open()) QFAIL("Unable to open file for testing");
    fileToRead.write(fileContent.toUtf8());
    fileToRead.close();

    QJsonObject result;
    ProjectFileIO io;
    io.readFile(fileToRead.fileName(), result);
    QString stringvalue = result["string"].toString();
    QString obj1name = result["arrayofobjs"][0]["name"].toString();
    QString obj2name = result["arrayofobjs"][1]["name"].toString();

    QCOMPARE(stringvalue,"string");
    QCOMPARE(obj1name,"obj1");
    QCOMPARE(obj2name,"obj2");
}


void ProjectFileIOTest::writeFileSlotWritesValidJsonToFile()
{
    QTemporaryFile fileToWrite("figurer-projectfileio-writefile.json");
    fileToWrite.open(); fileToWrite.close();
    QString jsonContent = QString(R"(
    {"string":"string", "arrayofobjs":[{"name":"obj1"},{"name":"obj2"}]}
    )");
    QJsonObject jsonToWrite = QJsonDocument::fromJson(jsonContent.toUtf8()).object();


    ProjectFileIO io;
    io.writeFile(fileToWrite.fileName(), jsonToWrite);
    if (!fileToWrite.open()) QFAIL("Unable to open file for testing");
    QJsonObject jsonFromFile = QJsonDocument::fromJson(fileToWrite.readAll()).object();
    fileToWrite.close();

    QString obj1name = jsonFromFile["arrayofobjs"][0]["name"].toString();
    QString obj2name = jsonFromFile["arrayofobjs"][1]["name"].toString();

    QCOMPARE(jsonFromFile["string"].toString(), jsonToWrite["string"].toString());
    QCOMPARE(obj1name,"obj1");
    QCOMPARE(obj2name,"obj2");
}

QTEST_MAIN(ProjectFileIOTest)
#include "projectfileio_test.moc"
