#include <QTest>
#include <QSignalSpy>
#include <QTemporaryFile>
#include <QJsonDocument>
#include <QJsonArray>

#include "../project.h"


class MockLatexParser : public QObject, public IExternalFileParser {
    Q_OBJECT;
public:
    MockLatexParser():QObject{nullptr} {

    }

    QJsonObject parse(QString& filePath) {
        QJsonObject ret;
        ret["includegraphics"] =
            QJsonArray::fromStringList(
            QStringList{"fig1.pdf","fig2.eps"});
        Q_EMIT parseTestSignal();
        return ret;
    }

    bool canParse(QString &filePath) {
        return true;
    }

Q_SIGNALS:
    void parseTestSignal();
};


class ProjectTest: public QObject
{
    Q_OBJECT;

private Q_SLOTS:
    void startFromExistingFileParams();
    void startFromNewFileParams();
    void consolidateFigList();

private:
    QSharedPointer<ProjectTools> makeMockTools(MockLatexParser* parser) {
        ProjectFileIO* io = new ProjectFileIO(this);
        ExternalFileScanner* scanner = new ExternalFileScanner(this);
        scanner->addParser(parser);
        QSharedPointer<ProjectTools> tools = QSharedPointer<ProjectTools>::create(io, scanner, this);
        return tools;
    };
};

void ProjectTest::startFromExistingFileParams()
{
    MockLatexParser* parser = new MockLatexParser();
    QSharedPointer<ProjectTools> tools = makeMockTools(parser);

    QTemporaryFile projFile("figurer-projfile-XXXXXXXXX.json");
    QTemporaryFile extFile("figurer-external-XXXXXXXXX.tex");
    extFile.open(); extFile.close();
    QString extPath = extFile.fileName();

    if (!projFile.open()) {
        QFAIL("Coudn't open temp project file for test");
    }
    QString projContent = QString(R"(
    { "externalFilePath": "%1" }
    )").arg(extPath);
    projFile.write(projContent.toUtf8());
    projFile.close();

    Project::ExistingFileParams params{projFile.fileName()};
    QSignalSpy spy(parser, &MockLatexParser::parseTestSignal);

    Project project(tools.get(), params);

    if (!projFile.open()) QFAIL("failed to reopen temp project file");
    QJsonDocument doc = QJsonDocument::fromJson(projFile.readAll());
    const QJsonArray &figsInJsonArray = doc.object()["figures"].toArray();
    projFile.close();

    qInfo() << "jsonarray: "<<figsInJsonArray;

    QString resultExtPath = project.getInfo("externalFilePath");

    bool allFigsMatch = true;
    for (const QJsonValue &value :figsInJsonArray) {
        if (!QStringList({"fig1.pdf","fig2.eps"}).contains(value["id"].toString())) {
            allFigsMatch = false;
            break;
        }
    }

    QCOMPARE(spy.count(),1);
    QCOMPARE(resultExtPath, extPath);
    QCOMPARE(project.projectFilePath, projFile.fileName());
    QVERIFY(extPath.isNull()==false);

    QVERIFY2(allFigsMatch, "all figures do not match with the ones in mock parser");
    delete parser;
}

void ProjectTest::startFromNewFileParams()
{
    MockLatexParser parser;
    QSharedPointer<ProjectTools> tools = makeMockTools(&parser);
    QTemporaryFile projFile("figurer-projfile-XXXXXXXXX.json");
    QTemporaryFile extFile("figurer-external-XXXXXXXXX.tex");
    bool openedFiles = extFile.open() && projFile.open();
    QString extPath = extFile.fileName();
    if (!openedFiles) QFAIL("Failed to setup files for test");
    projFile.close();  extFile.close();

    Project::NewFileParams params{projFile.fileName(), extFile.fileName()};
    QSignalSpy spy(&parser, &MockLatexParser::parseTestSignal);

    Project project(tools.get(), params);


    if (!projFile.open()) QFAIL("failed to reopen temp project file");
    QJsonDocument doc = QJsonDocument::fromJson(projFile.readAll());
    const QJsonArray &figsInJsonArray = doc.object()["figures"].toArray();
    projFile.close();

    qInfo() << "jsonarray: "<<figsInJsonArray;

    QString resultExtPath = project.getInfo("externalFilePath");

    bool allFigsMatch = true;
    for (const QJsonValue &value :figsInJsonArray) {
        if (!QStringList({"fig1.pdf","fig2.eps"}).contains(value["id"].toString())) {
            allFigsMatch = false;
            break;
        }
    }

    QCOMPARE(spy.count(),1);
    QCOMPARE(resultExtPath, extPath);
    QVERIFY(extPath.isNull()==false);
    QCOMPARE(project.projectFilePath, projFile.fileName());
    QVERIFY2(allFigsMatch, "all figures do not match with the ones in mock parser");
}

void ProjectTest::consolidateFigList()
{
    MockLatexParser parser;
    QSharedPointer<ProjectTools> tools = makeMockTools(&parser);
    QTemporaryFile projFile("figurer-projfile-XXXXXXXXX.json");
    QTemporaryFile extFile("figurer-external-XXXXXXXXX.tex");
    bool openedFiles = extFile.open() && projFile.open();
    if (!openedFiles) QFAIL("Failed to setup files for test");
    projFile.close();  extFile.close();

    Project::NewFileParams params{projFile.fileName(), extFile.fileName()};

    Project project(tools.get(), params);
    QSignalSpy spy(&project, SIGNAL(foundDanglingIds(QStringList)));
    QStringList listToConsolidate({"fig1.pdf", "anotherfig1.eps", "anotherfig2.pdf"});

    project.consolidateFigureList(listToConsolidate);

    if (!projFile.open()) QFAIL("failed to reopen temp project file");
    QJsonDocument doc = QJsonDocument::fromJson(projFile.readAll());
    const QJsonArray &figsInJsonArray = doc.object()["figures"].toArray();
    projFile.close();
    QStringList listFromFile;

    for (const QJsonValue &value :figsInJsonArray) {
        listFromFile.append(value["id"].toString());
    }

    qInfo() << "Consolidate test:" << spy.count();
    QList<QVariant> arguments = spy.first();
    QStringList danglingList = arguments.at(0).value<QStringList>();

    QVERIFY(listFromFile.contains("fig1.pdf"));
    QVERIFY(listFromFile.contains("fig2.eps"));
    QVERIFY(listFromFile.contains("anotherfig1.eps"));
    QVERIFY(listFromFile.contains("anotherfig2.pdf"));
    QVERIFY2(listFromFile.count("fig1.pdf")==1, "Existing figure in project should not duplicate on consolidation");
    QVERIFY2(spy.count()==1, "Dangling ids should be signaled");
    QVERIFY2(danglingList.contains("fig2.eps"), "Dangling id should be available through signal argument");

}



QTEST_MAIN(ProjectTest)
#include "project_test.moc"
