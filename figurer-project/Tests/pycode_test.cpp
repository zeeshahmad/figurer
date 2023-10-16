
#include <QString>
#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QFutureWatcher>

#include "pycode.h"

namespace pyc=pycode;

class PycodeTest: public QObject
{
    Q_OBJECT;

private Q_SLOTS:
    //the namespace will have worker and job
    void workerThreadDestructionNoError();
    void getIntResultSync();
    void getIntResultAsync();
    void getStringResultSync();
    void getStringResultAsync();
    void getErrorFromPython();
    void incorectPythonVarName();
    void unrecognisedResultType();
    void queueAndGetTenResults();
    void cancelCodesWithTag();
private:
};

void PycodeTest::workerThreadDestructionNoError()
{
    pyc::Worker worker;
    pyc::Worker worker2;
    worker2.start();

    QVERIFY2(true, "Should be no problem when worker ends");
}


void PycodeTest::getIntResultSync()
{
    pyc::Worker worker;
    worker.start();

    pyc::Result result = worker
        .enqueue("result=1+3", "result", "tagname").result();

    QCOMPARE(result.toInt(), 4);
    QVERIFY(result.success());
}

void PycodeTest::getIntResultAsync()
{
    pyc::Worker worker;

    QFutureWatcher<pyc::Result> watcher;
    QSignalSpy spy(&watcher, SIGNAL(finished()));

    QFuture<pyc::Result> result = worker.enqueue("result=1+3", "result");
    watcher.setFuture(result);
    int intResult; bool success = false;
    result.then([&intResult, &success](pyc::Result res) {
        intResult = res.toInt();
        success = res.success();
    });
    worker.start();
    spy.wait(2000);
    QCOMPARE(intResult, 4);
    QVERIFY(success);
}

void PycodeTest::getStringResultSync()
{
    pyc::Worker worker;
    worker.start();

    pyc::Result result = worker
        .enqueue("result='ahem ahem!'", "result", "tagname").result();

    QCOMPARE(result.toString(), "ahem ahem!");
    QVERIFY(result.success());
}

void PycodeTest::getStringResultAsync()
{
    pyc::Worker worker;

    QFutureWatcher<pyc::Result> watcher;
    QSignalSpy spy(&watcher, SIGNAL(finished()));

    QFuture<pyc::Result> future = worker.enqueue("result='this is a string!'", "result");
    watcher.setFuture(future);
    QString result; bool success;
    future.then([&result, &success](pyc::Result res) {
        result = res.toString();
        success = res.success();
    });
    worker.start();
    spy.wait(2000);
    QCOMPARE(result, "this is a string!");
    QVERIFY(success);
}

void PycodeTest::getErrorFromPython()
{
    pyc::Worker worker;
    worker.start();
    QString codestring = R"(
result = SOMETHING INCORRECT
    )";

    pyc::Result result = worker.enqueue(codestring, "result", "tagname").result();

    QString errortext = result.error();
    bool failure = !result.success();

    QVERIFY(failure);
    QVERIFY(errortext.startsWith("std::exception whilst python execution: "));
}

void PycodeTest::incorectPythonVarName()
{
    pyc::Worker worker;
    worker.start();
    QString codestring = R"(
result = 12
    )";

    pyc::Result result = worker.enqueue(codestring, "resultincorrectspelling", "tagname").result();

    QString errortext = result.error();
    bool failure = !result.success();

    QVERIFY(failure);
    QVERIFY(errortext.startsWith("std::exception whilst python execution: NameError: name 'resultincorrectspelling' is not defined"));
}

void PycodeTest::unrecognisedResultType()
{
    pyc::Worker worker;
    worker.start();
    QString codestring = R"(
class DummyType:
    pass
result = DummyType()
    )";

    pyc::Result result = worker.enqueue(codestring, "result").result();

    QString errortext = result.error();
    bool failure = !result.success();

    QVERIFY(failure);
    QCOMPARE(errortext, "Could not convert type DummyType from python code");
}

void PycodeTest::queueAndGetTenResults()
{
    pyc::Worker worker;
    worker.start();

    QList<QFuture<pyc::Result>> futures;
    for (int i=0; i< 10; i++) {
        QString codestring = QString(R"(
import time
time.sleep(.1)
result = %1**3)").arg(i);
        futures.append(worker.enqueue(codestring, "result"));
    }

    for (int i=0; i<10; i++) {
        int result = futures.at(i).result().toInt();
        QCOMPARE(result, i*i*i);
    }
}

void PycodeTest::cancelCodesWithTag()
{
    pyc::Worker worker;

    int result1 = -1;
    int result2 = -1;
    int result3 = -1;
    QString error1, error2, error3;

    QFuture<pyc::Result> future1 = worker.enqueue("result=1", "result", "tag1");
    QFuture<pyc::Result> future2 = worker.enqueue("result=2", "result", "tag1");
    QFuture<pyc::Result> future3 = worker.enqueue("result=3", "result", "tag2");

    worker.cancelCodesWithTag("tag1");
    worker.start();
    error1 = future1.result().error();
    error2 = future2.result().error();
    error3 = future3.result().error();
    if (future1.result().success()) result1 = future1.result().toInt();
    if (future2.result().success()) result2 = future2.result().toInt();
    if (future3.result().success()) result3 = future3.result().toInt();

    QCOMPARE(result1,-1); QCOMPARE(error1, "Cancelled");
    QCOMPARE(result2,-1); QCOMPARE(error2, "Cancelled");
    QCOMPARE(result3,3); QCOMPARE(error3, "0");
}


QTEST_MAIN(PycodeTest)
#include "pycode_test.moc"
