#include <QTest>
#include <QSignalSpy>
#include <QTemporaryFile>
#include <QJsonDocument>
#include <QJsonArray>

#include "projectmanager.h"

class DummyParser: public IExternalFileParser
{
public:
    QList<QString> parseCalls;
    QJsonObject parse(const QString& filePath) {
        parseCalls.append(filePath);
        return QJsonDocument::fromJson(QString(R"(
            {"includegraphics":["item1", "item2", "item3"]}
        )").toUtf8()).object();
    }

    bool canParse(const QString& filePath) {
        return !filePath.contains("invalidtype");
    }
};


class ProjectManagerTest: public QObject
{
    Q_OBJECT;

private Q_SLOTS:
    void testIsAProjectOpen();
    void cantOpenProjectWhenAProjectIsAlreadyOpen();
    void cantCreateProjectWhenAProjectIsAlreadyOpen();
    void cantCloseProjectWhenNoProjectIsOpen();

    void testCreateProject();
    void testOpenProject();
    void testCloseProject();

private:
    QSharedPointer<ProjectTools> makeProjectTools(IExternalFileParser *parser) {
        ProjectFileIO* io = new ProjectFileIO(this);
        ExternalFileScanner* scanner = new ExternalFileScanner(this);

        scanner->addParser(parser);
        QSharedPointer<ProjectTools> tools = QSharedPointer<ProjectTools>::create(io, scanner);
        return tools;
    };
};


void ProjectManagerTest::testIsAProjectOpen()
{
    DummyParser parser;
    QSharedPointer<ProjectTools> tools = makeProjectTools(&parser);
    ProjectManager manager(tools.get());

    bool beforeOpeningProject = manager.isAProjectOpen();
    QString path = "some path";
    manager.openProjectRequested(path);
    bool afterOpeningProject = manager.isAProjectOpen();
    manager.closeProjectRequested();
    bool afterClosingProject = manager.isAProjectOpen();
    QVERIFY2(!beforeOpeningProject, "should return false before any project is opened");
    QVERIFY2(afterOpeningProject, "should return true after a project is opened");
    QVERIFY2(!afterClosingProject, "should return false again after a project is closed");
}

void ProjectManagerTest::cantOpenProjectWhenAProjectIsAlreadyOpen()
{
    DummyParser parser;
    QSharedPointer<ProjectTools> tools = makeProjectTools(&parser);
    ProjectManager manager(tools.get());
    QSignalSpy spy(&manager, SIGNAL(error_projectalreadyopen(QString)));

    QString path = "some path";
    manager.openProjectRequested(path);
    QSignalSpy spy2(&manager, SIGNAL(projectOpened(QString)));
    manager.openProjectRequested(path);
    QString errortext = spy.first().first().toString();

    QCOMPARE(spy.count(),1);
    QCOMPARE(errortext, "Can't open a project when another project is already open!");
    QCOMPARE(spy2.count(),0);
}

void ProjectManagerTest::cantCreateProjectWhenAProjectIsAlreadyOpen()
{
    DummyParser parser;
    QSharedPointer<ProjectTools> tools = makeProjectTools(&parser);
    ProjectManager manager(tools.get());
    QSignalSpy spy(&manager, SIGNAL(error_projectalreadyopen(QString)));

    QString path = "some path";
    QString path2 = "some path2";
    manager.openProjectRequested(path);
    QSignalSpy spy2(&manager, SIGNAL(projectOpened(QString)));
    manager.createProjectRequested(path, path2);
    QString errortext = spy.first().first().toString();

    QCOMPARE(spy.count(),1);
    QCOMPARE(errortext, "Can't create a new project when another project is already open!");
    QCOMPARE(spy2.count(),0);
}

void ProjectManagerTest::cantCloseProjectWhenNoProjectIsOpen()
{
    DummyParser parser;
    QSharedPointer<ProjectTools> tools = makeProjectTools(&parser);
    ProjectManager manager(tools.get());
    QSignalSpy spy(&manager, SIGNAL(error_noprojecttoclose(QString)));
    QSignalSpy spy2(&manager, SIGNAL(projectClosed()));

    manager.closeProjectRequested();
    QString errortext = spy.first().first().toString();

    QCOMPARE(spy.count(),1);
    QCOMPARE(errortext, "No project is open to be able to close!");
    QCOMPARE(spy2.count(),0);
}

void ProjectManagerTest::testCreateProject()
{
    DummyParser parser;
    QSharedPointer<ProjectTools> tools = makeProjectTools(&parser);
    ProjectManager manager(tools.get());
    QSignalSpy spy(&manager, SIGNAL(projectOpened(QString)));

    QString pathToNewFile = "new file path";
    QString pathToExternalFile = "external file path";

    manager.createProjectRequested(pathToNewFile, pathToExternalFile);
    QString recoveredExternalFilePath = spy.first().first().toString();

    QCOMPARE(spy.count(),1);
    QCOMPARE(recoveredExternalFilePath, "external file path");
    QVERIFY(manager.isAProjectOpen());
}

void ProjectManagerTest::testOpenProject()
{
    DummyParser parser;
    QSharedPointer<ProjectTools> tools = makeProjectTools(&parser);
    ProjectManager manager(tools.get());
    QSignalSpy spy(&manager, SIGNAL(projectOpened(QString)));

    QString pathToOpenFile = "open file path";

    manager.openProjectRequested(pathToOpenFile);
//    QString recoveredExternalFilePath = spy.first().first().toString();//could somehow make a mock project or something to test argument
    //projectmanager and project are tightly coupled

    QCOMPARE(spy.count(),1);
}

void ProjectManagerTest::testCloseProject()
{
    DummyParser parser;
    QSharedPointer<ProjectTools> tools = makeProjectTools(&parser);
    ProjectManager manager(tools.get());
    QSignalSpy spy(&manager, SIGNAL(projectClosed()));

    QString pathToOpenFile = "open file path";
    manager.openProjectRequested(pathToOpenFile);
    manager.closeProjectRequested();

    QCOMPARE(spy.count(),1);
    QVERIFY2(manager.isAProjectOpen()==false, "project is closed");
}

QTEST_MAIN(ProjectManagerTest)
#include "projectmanager_test.moc"
