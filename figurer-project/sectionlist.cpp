#include "sectionlist.h"
#include <QDebug>
#include <QListWidget>
#include <QSignalBlocker>

SectionList::SectionList(QWidget* parent):QListWidget{parent}
{
    addMandatoryItems();

    setCurrentRow(0);
    connect(this, &QListWidget::currentItemChanged, [&](QListWidgetItem* current, QListWidgetItem* prev){
        Q_EMIT sectionSelected(current->text());
    });
}

void SectionList::resetSections(const QStringList & list)
{
//    this->clearSelection();
    {
        QSignalBlocker blocker(this);
        this->clear(); //https://www.appsloveworld.com/cplus/100/277/qlistwidget-causes-crash-when-clear-is-called-while-items-are-selected
    }

    addMandatoryItems();
    for (int i=0; i< list.count(); i++) this->addItem(list.at(i));

    QStringList totalList;
    for (int i=0; i< this->count(); i++) totalList.append(this->item(i)->text());

    Q_EMIT sectionsReset(totalList);
}

void SectionList::addMandatoryItems()
{
    this->addItem(":pre-python");
    this->addItem(":post-python");
}
