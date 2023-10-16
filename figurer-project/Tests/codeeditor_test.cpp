#include <QTest>
#include <QSignalSpy>
#include <QDebug>

#include "codeeditor.h"

class CodeEditorTest: public QObject
{
    Q_OBJECT;

private Q_SLOTS:

    void emitsCodeOnChange();
};

void CodeEditorTest::emitsCodeOnChange()
{
    CodeEditor editor;
    QSignalSpy spy(&editor, SIGNAL(codeChanged(const QString&)));
    QString text("this is a test text");
    editor.setText(text);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().toString(), text);
}


QTEST_MAIN(CodeEditorTest)
#include "codeeditor_test.moc"
