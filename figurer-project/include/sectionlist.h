#ifndef SECTIONLIST_H
#define SECTIONLIST_H

#include <QListWidget>

class SectionList : public QListWidget
{
    Q_OBJECT
public:
    SectionList(QWidget *parent=nullptr);
Q_SIGNALS:
    void sectionSelected(const QString& sectionLabel);
    void sectionsReset(const QStringList&);
public Q_SLOTS:
    void resetSections(const QStringList&);
private:
    void addMandatoryItems();
};

#endif // SECTIONLIST_H
