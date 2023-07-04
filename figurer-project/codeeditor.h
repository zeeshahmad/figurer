#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QFile>

// QCodeEditor
#include <QCodeEditor>
#include <QPythonCompleter>
#include <QSyntaxStyle>
#include <QPythonHighlighter>

class CodeEditor : public QCodeEditor
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);

private Q_SLOTS:
    void onTextChanged();

private:
    void initData();

    void loadStyle(QString path);

//    QString loadCode(QString path);


    QVector<QPair<QString, QString>> m_codeSamples;
    QCompleter* completer;
    QStyleSyntaxHighlighter* highlighter;
    QSyntaxStyle* syntax_style;
};

#endif // CODEEDITOR_H
