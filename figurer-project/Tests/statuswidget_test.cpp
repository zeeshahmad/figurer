#include <QTest>
#include <QSignalSpy>

#include "../statuswidget.h"

class StatusWidgetTest: public QObject
{
    Q_OBJECT;

private Q_SLOTS:
    void restartCooldownSimple();

private:

};

void StatusWidgetTest::restartCooldownSimple()
{
    StatusWidget sw;
    QString samplecode = "sample code";
    QSignalSpy spy(&sw, SIGNAL(cooldownCompleted(QString*)));

    sw.restartCooldown(&samplecode);//probably producing leaks for Qstring*, check!

    spy.wait();

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(StatusWidgetTest)
#include "statuswidget_test.moc"
