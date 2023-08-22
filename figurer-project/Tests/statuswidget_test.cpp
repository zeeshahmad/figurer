#include <QTest>

#include "../statuswidget.h"

class StatusWidgetTest: public QObject
{
    Q_OBJECT;

private Q_SLOTS:
    void test1();
};

void StatusWidgetTest::test1()
{
    StatusWidget sw;
    QCOMPARE("haha","haha");
}

QTEST_MAIN(StatusWidgetTest)
#include "statuswidget_test.moc"
