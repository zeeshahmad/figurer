#include <QTest>
#include <QSignalSpy>
#include <QDebug>

#include "sectionlist.h"

class SectionListTest: public QObject
{
    Q_OBJECT;

private Q_SLOTS:
    void containsPrePostCodeItems();
    void firstItemSelectedAtStart();
    void multiSelectionNotPossible();
    void signalForSelectionChanged();
    void resetSections();
};

void SectionListTest::containsPrePostCodeItems()
{
    SectionList sl;

    QCOMPARE(sl.item(0)->text(), ":pre-python");
    QCOMPARE(sl.item(1)->text(), ":post-python");
}

void SectionListTest::firstItemSelectedAtStart()
{
    SectionList sl;
    int selectCount = sl.selectedItems().count();
    QListWidgetItem* firstItem = sl.item(0);
    QListWidgetItem* selectedItem = sl.selectedItems().at(0);

    QCOMPARE(selectCount, 1);
    QCOMPARE(firstItem, selectedItem);
}

void SectionListTest::multiSelectionNotPossible()
{
    SectionList sl;
    QAbstractItemView::SelectionMode mode = sl.selectionMode();

    QCOMPARE(mode, QAbstractItemView::SingleSelection);
}

void SectionListTest::signalForSelectionChanged()
{
    SectionList sl;
    QSignalSpy spy(&sl, SIGNAL(sectionSelected(QString)));

    sl.setCurrentRow(1);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0), ":post-python");
}

void SectionListTest::resetSections()
{
    SectionList sl;
    QSignalSpy spy(&sl, SIGNAL(sectionsReset(QStringList)));
    QStringList sectionNames = {"test 1", "test2"};

    QStringList expectedList = {":pre-python",":post-python","test 1", "test2"};

    sl.resetSections(sectionNames);

    QStringList actualList;
    for (int i =0; i< sl.count(); i++) {
        actualList.append(sl.item(i)->text());
        qInfo() << "in for: " << sl.item(i)->text();
    }

    QStringList listFromSpy = spy.at(0).at(0).toStringList();

    QCOMPARE(actualList, expectedList);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(listFromSpy, expectedList);
}


QTEST_MAIN(SectionListTest)
#include "sectionlist_test.moc"
