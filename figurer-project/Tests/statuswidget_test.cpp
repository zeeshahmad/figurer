#include <QTest>
#include <QSignalSpy>
#include <QDebug>

#include "../statuswidget.h"

class StatusWidgetTest: public QObject
{
    Q_OBJECT;

private Q_SLOTS:
    void restartCooldownSimple();
    void restartCooldownTwiceQuickly();
    void restartCooldownTwiceSlowly();
    void testUpdateBarToEmit();
    void testUpdateBarEmitManyTimes();
};

void StatusWidgetTest::restartCooldownSimple()
{
    StatusWidget sw;
    QString samplecode = "sample code";
    QSignalSpy spy(&sw, SIGNAL(cooldownCompleted(QString*)));


    const QDateTime methodCallTime = QDateTime::currentDateTime();
    sw.restartCooldown(&samplecode);
    spy.wait(1000); //wait atmost 1 second
    const QDateTime signalTime = QDateTime::currentDateTime();


    QList<QVariant> arguments = spy.first();
    QString* recoveredcode = arguments.at(0).value<QString*>();

    QCOMPARE(spy.count(), 1); //signal emitted after cooldown completed
    QVERIFY(methodCallTime.msecsTo(signalTime) >= 500); //signal emit took atleast ~500ms
    QCOMPARE(*recoveredcode, samplecode);//signal transports code string
}

void StatusWidgetTest::restartCooldownTwiceQuickly()
{
    StatusWidget sw;
    QString somecode = "somecode";
    QString replacementcode = "some new code";
    QSignalSpy spy(&sw, SIGNAL(cooldownCompleted(QString*)));

    sw.restartCooldown(&somecode);
    QTest::qWait(50);
    sw.restartCooldown(&replacementcode);
    spy.wait(1000);

    QList<QVariant> arguments = spy.first();
    QString* recoveredcode = arguments.at(0).value<QString*>();

    QCOMPARE(spy.count(),1); //signal exactly once
    QCOMPARE(*recoveredcode, replacementcode);//signal recovers replaced code, not the first one
}

void StatusWidgetTest::restartCooldownTwiceSlowly()
{
    StatusWidget sw;
    QString somecode = "somecode";
    QString secondcode = "some new code";
    QSignalSpy spy(&sw, SIGNAL(cooldownCompleted(QString*)));

    sw.restartCooldown(&somecode);
    spy.wait(1000);
    QTest::qWait(50);
    sw.restartCooldown(&secondcode);
    spy.wait(1000);

    QList<QVariant> arguments1 = spy.at(0);
    QList<QVariant> arguments2 = spy.at(1);
    QString* recoveredcode1 = arguments1.at(0).value<QString*>();
    QString* recoveredcode2 = arguments2.at(0).value<QString*>();

    QCOMPARE(spy.count(),2); //signal exactly 2wice
    QCOMPARE(*recoveredcode1, somecode);//recovers both code
    QCOMPARE(*recoveredcode2, secondcode);//recovers both code
}

void StatusWidgetTest::testUpdateBarToEmit()
{
    StatusWidget sw;
    QSignalSpy spy(&sw, SIGNAL(cooldownCompleted(QString*)));
    int numCallsToEmit = 50;

    for (int i=0; i<numCallsToEmit; i++)
    {
        sw.updateBar();
    }

    int countJustBeforeEmit = spy.count();
    sw.updateBar();
    int countAfterEmit = spy.count();

    QCOMPARE(countJustBeforeEmit, 0);
    QCOMPARE(countAfterEmit, 1);

}

void StatusWidgetTest::testUpdateBarEmitManyTimes()
{
    StatusWidget sw;
    QSignalSpy spy(&sw, SIGNAL(cooldownCompleted(QString*)));
    int numCalls = 327;

    for (int i=0; i<numCalls; i++)
    {
        sw.updateBar();
    }

    int emitCount = spy.count();

    QCOMPARE(emitCount, numCalls/50);

}


QTEST_MAIN(StatusWidgetTest)
#include "statuswidget_test.moc"
