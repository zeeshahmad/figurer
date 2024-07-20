#include <QTest>
#include <QSignalSpy>
#include <QDebug>
#include <QHash>

#include "codeeditor.h"

class CodeEditorTest: public QObject
{
    Q_OBJECT;

private Q_SLOTS:

    void emitStitchedCodeOnChange();
    void showBuffer();
    void overwriteExistingBuffersShouldMerge();
    void getBufferIds();
    void getBufferText();
//    void deleteAllBuffers();

private:
    QHash<QString, QString>  dummyBufferData(const QString key="key1", const QString val = "some text") {
        QHash<QString, QString> data;
        data[key] = val;
        data[":pre-python"] = "pre python";
        data[":post-python"] = "post python";
        return data;
    }
};

void CodeEditorTest::emitStitchedCodeOnChange()
{
    CodeEditor editor;
    editor.overwriteBuffers(dummyBufferData());
    editor.showBuffer("key1");
    QSignalSpy spy(&editor, SIGNAL(codeChanged(QString)));
    QString text("this is a test text");
    editor.setText(text);

    QString expectedCode = "pre python\nthis is a test text\npost python";
    QString actualCode = spy.first().first().toString();

    QCOMPARE(spy.count(), 1);
    QCOMPARE(actualCode, expectedCode);
}

void CodeEditorTest::showBuffer()
{
    CodeEditor editor;
    editor.overwriteBuffers(dummyBufferData());
    QSignalSpy spy(&editor, SIGNAL(codeChanged(QString)));
    editor.showBuffer("key1");
    QString textShown = editor.toPlainText();

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().toString(), "pre python\nsome text\npost python");
    QCOMPARE(textShown, "some text");

}

void CodeEditorTest::overwriteExistingBuffersShouldMerge()
{
    CodeEditor editor;

    editor.overwriteBuffers(dummyBufferData());
    editor.overwriteBuffers(dummyBufferData("key2", "some other text"));
    CodeEditor::BufferData otherdata;
    otherdata["key2"] = "overwritten";
    otherdata["key3"] = "newdata";

    editor.overwriteBuffers(otherdata);

    editor.showBuffer("key1");
    QString key1text = editor.toPlainText();
    editor.showBuffer("key2");
    QString key2text = editor.toPlainText();
    editor.showBuffer("key3");
    QString key3text = editor.toPlainText();

    QCOMPARE(key1text, "some text");
    QCOMPARE(key2text, "overwritten");
    QCOMPARE(key3text, "newdata");
}

void CodeEditorTest::getBufferIds()
{
    CodeEditor editor;

    editor.overwriteBuffers(dummyBufferData());
    editor.overwriteBuffers(dummyBufferData("key2", "some other text"));

    QStringList expected = {"key1", "key2", ":pre-python", ":post-python"};
    QStringList ids = editor.getBufferIds();
    expected.sort();
    ids.sort();

    QCOMPARE(ids, expected);
}

void CodeEditorTest::getBufferText()
{
    CodeEditor editor;

    editor.overwriteBuffers(dummyBufferData());

    QString bufferText = editor.getBufferText("key1");

    QCOMPARE(bufferText, "some text");
}

//void CodeEditorTest::deleteAllBuffers()
//{
//    CodeEditor editor;

//    editor.overwriteBuffers(dummyBufferData());
//    editor.overwriteBuffers(dummyBufferData("key2", "some other text"));

//}


QTEST_MAIN(CodeEditorTest)
#include "codeeditor_test.moc"
