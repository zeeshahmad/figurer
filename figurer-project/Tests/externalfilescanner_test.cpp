#include <QTest>
#include <QSignalSpy>
#include <QTemporaryFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "externalfilescanner.h"


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

class ExternalFileScannerTest: public QObject
{
    Q_OBJECT;

private Q_SLOTS:
    void scanCallsParseOnScanner();
    void scanReturnsListOfIncludegraphics();
    void scanSlotCallsScanMethod();
    void scanSignalsErrorWhenNoParserAdded();
    void scanSignalsErrorWhenNoSuitableParserAvailable();
private:

};


void ExternalFileScannerTest::scanCallsParseOnScanner()
{
    ExternalFileScanner scanner;
    DummyParser dummyparser;
    scanner.addParser(&dummyparser);

    scanner.scan("some path");
    int numberOfParserCalls = dummyparser.parseCalls.count();
    QString recoveredFilePath = dummyparser.parseCalls[0];

    QCOMPARE(numberOfParserCalls,1);
    QCOMPARE(recoveredFilePath, "some path");
}

void ExternalFileScannerTest::scanReturnsListOfIncludegraphics()
{
    ExternalFileScanner scanner;
    DummyParser parser;
    scanner.addParser(&parser);

    QStringList result = scanner.scan("some path");

    QCOMPARE(result.count(), 3);
    QCOMPARE(result[0], "item1");
    QCOMPARE(result[1], "item2");
    QCOMPARE(result[2], "item3");
}

void ExternalFileScannerTest::scanSlotCallsScanMethod()
{
    ExternalFileScanner scanner;
    DummyParser parser;
    scanner.addParser(&parser);

    QStringList dummyresult;
    scanner.scanSlot("some path", dummyresult);

    int numberOfParserCalls = parser.parseCalls.count();
    QString recoveredFilePath = parser.parseCalls[0];

    QCOMPARE(numberOfParserCalls,1);
    QCOMPARE(recoveredFilePath, "some path");

    QCOMPARE(dummyresult.count(), 3);
    QCOMPARE(dummyresult[0], "item1");
    QCOMPARE(dummyresult[1], "item2");
    QCOMPARE(dummyresult[2], "item3");
}

void ExternalFileScannerTest::scanSignalsErrorWhenNoParserAdded()
{
    ExternalFileScanner scanner;
    QSignalSpy spy(&scanner, SIGNAL(error_nosuitableparser(QString)));


    scanner.scan("some path");
    QString errortext = spy.first().first().toString();

    QCOMPARE(spy.count(), 1);
    QCOMPARE(errortext, "Could not choose a suitable parser for file: some path");
}

void ExternalFileScannerTest::scanSignalsErrorWhenNoSuitableParserAvailable()
{
    ExternalFileScanner scanner;
    DummyParser parser;
    scanner.addParser(&parser);
    QSignalSpy spy(&scanner, SIGNAL(error_nosuitableparser(QString)));


    scanner.scan("invalidtype");
    QString errortext = spy.first().first().toString();

    QCOMPARE(spy.count(), 1);
    QCOMPARE(errortext, "Could not choose a suitable parser for file: invalidtype");
}




QTEST_MAIN(ExternalFileScannerTest)
#include "externalfilescanner_test.moc"
