#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QFile>

// QCodeEditor
#include <QCodeEditor>
#include <QPythonCompleter>
#include <QSyntaxStyle>
#include <QPythonHighlighter>
#include <QSharedPointer>
#include <QHash>
#include <QHashIterator>

class CodeEditor : public QCodeEditor
{
    Q_OBJECT

public:
    using BufferData = QHash<QString, QString>;

    CodeEditor(QWidget *parent = 0);
    QStringList getBufferIds();
    const QString getBufferText(const QString& bufferId) const;

Q_SIGNALS:
    void codeChanged(const QString&);
public Q_SLOTS:
    void showBuffer(const QString& bufferId);
    void overwriteBuffers(const BufferData& newData);
private Q_SLOTS:
    void onTextChanged();
private:
    using BufferDataIterator = QHashIterator<QString, QString>;
    void initData();

    void loadStyle(QString path);

    QHash<QString, QString> buffers;//<tab id, code text>
    QString visibleBufferId;
    QString& visibleBufferText();
    bool visibleBufferExists();
    void visibleBufferToEmptyId();
    QString stitchTextForBuffer(const QString& bufferId);
    QString stitchTextForVisibleBuffer();
    bool bufferIsFigure(const QString &bufferId);

    QVector<QPair<QString, QString>> m_codeSamples;
    QCompleter* completer;
    QStyleSyntaxHighlighter* highlighter;
    QSyntaxStyle* syntax_style;
};

#endif // CODEEDITOR_H
