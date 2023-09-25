#include <QTest>
#include <QSignalSpy>
#include <QTemporaryFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>

#include "pythonthread.h"

class PythonThreadTest: public QObject
{
    Q_OBJECT;

private Q_SLOTS:
    void objectNameTest();
    void getsStringResult();
    void getsIntResult();
    void getsErrorFromPython();
    void incorectPythonVarName();
    void unrecognisedResultType();
    void getsResultAsync();
    void queueAndGetTenResults();
    void cancelsCodesWithTag();
};

void PythonThreadTest::objectNameTest()
{
    PythonThread pt;
    QCOMPARE(pt.objectName(), "python thread");
}

void PythonThreadTest::getsStringResult()
{
    PythonThread pt;
    pt.start();
    QString codestring = R"(
sum = 23+5
result = str(sum)
    )";
    PythonThread::Code code(codestring, "result");
    pt.addToQueue(code);
    QString result;
    QBENCHMARK {
        result = code.result().toString();
    }


    QCOMPARE(result, "28");
}

void PythonThreadTest::getsIntResult()
{
    PythonThread pt;
    pt.start();
    QString codestring = R"(
sum = 23+5
result = sum
    )";
    PythonThread::Code code(codestring, "result");
    pt.addToQueue(code);
    int result = -1;
    QBENCHMARK {
        result = code.result().toInt();
    }

    QCOMPARE(result, 28);
}

void PythonThreadTest::getsErrorFromPython()
{
    PythonThread pt;
    pt.start();
    QString codestring = R"(
result = SOMETHING INCORRECT
    )";
    PythonThread::Code code(codestring, "result");
    pt.addToQueue(code);
    QString errortext;
    if (!code.successAfterWaiting()) {
        errortext = code.error;
    }
    QVERIFY(errortext.startsWith("std::exception whilst python execution: "));
}

void PythonThreadTest::incorectPythonVarName()
{
    PythonThread pt;
    pt.start();
    QString codestring = R"(
result = 12
    )";
    PythonThread::Code code(codestring, "resultincorrectspelling");
    pt.addToQueue(code);
    QString errortext;
    if (!code.successAfterWaiting()) {
        errortext = code.error;
    }
    QVERIFY(errortext.startsWith("std::exception whilst python execution: NameError: name 'resultincorrectspelling' is not defined"));
}

void PythonThreadTest::unrecognisedResultType()
{
    PythonThread pt;
    pt.start();
    QString codestring = R"(
class DummyType:
    pass
result = DummyType()
    )";
    PythonThread::Code code(codestring, "result");
    pt.addToQueue(code);
    QString errortext;
    if (!code.successAfterWaiting()) {
        errortext = code.error;
    }
    QCOMPARE(errortext, "Could not convert type DummyType from python code");
}

void PythonThreadTest::getsResultAsync()
{
    PythonThread pt;
    pt.start();
    QString codestring = R"(
sum = 23+5
import time
time.sleep(.2)
result = sum
    )";
    PythonThread::Code code(codestring, "result");
    PythonThread::Watcher watcher;
    QSignalSpy spy(&watcher, &PythonThread::Watcher::finished);

    pt.addToQueue(code, &watcher);
    spy.wait();
    int result = watcher.result().toInt();

    QCOMPARE(result, 28);
    QCOMPARE(spy.count(),1);
}

void PythonThreadTest::queueAndGetTenResults()
{
    PythonThread pt;
    pt.start();
    QList<PythonThread::Code*> codes;
    for (int i=0; i< 10; i++) {
        QString codestring = QString(R"(
result = %1**3
    )").arg(i);

        PythonThread::Code* code = new PythonThread::Code(codestring, "result");
        pt.addToQueue(*code);
        codes.append(code);
    }

    QList<int> results;

    QBENCHMARK {
        for (int i=0; i<10; i++) {
            PythonThread::Code* code = codes.at(i);
            results.append( code->result().toInt() );
        }
    }

    for (int i=0; i<10; i++) {
        delete codes.at(i);
        QCOMPARE(results.at(i), i*i*i);
    }
}

void PythonThreadTest::cancelsCodesWithTag()
{
    PythonThread pt;
    PythonThread::Code code1(R"(result = "code1")", "result", "tagname");
    PythonThread::Code code2(R"(result = "code2")", "result", "tagname");
    PythonThread::Code code3(R"(result = "code3")", "result", "anothertagname");
    PythonThread::Watcher watcher1;
    PythonThread::Watcher watcher2;
    PythonThread::Watcher watcher3;
    QSignalSpy spy1(&watcher1, &PythonThread::Watcher::finished);
    QSignalSpy spy2(&watcher2, &PythonThread::Watcher::finished);
    QSignalSpy spy3(&watcher3, &PythonThread::Watcher::finished);


    pt.addToQueue(code1, &watcher1);
    pt.addToQueue(code2, &watcher2);
    pt.addToQueue(code3, &watcher3);

    pt.cancelCodesWithTag("tagname");
    pt.start();
    spy3.wait();

    QString result3 = code3.result().toString();

    QCOMPARE(result3, "code3");
    QCOMPARE(spy1.count(),1);
    QCOMPARE(spy2.count(),1);
    QCOMPARE(spy3.count(),1);
    QCOMPARE(code1.error, "Cancelled");
    QCOMPARE(code2.error, "Cancelled");
    QVERIFY(code3.successAfterWaiting());
}


QTEST_MAIN(PythonThreadTest)
#include "pythonthread_test.moc"
