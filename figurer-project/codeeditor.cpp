#include "codeeditor.h"
#include <QDebug>

CodeEditor::CodeEditor(QWidget *parent): QCodeEditor(parent)
{
    qInfo("constructor!");
    initData();

    //this->setPlainText  (m_codeSamples[0].second);
    this->setSyntaxStyle(syntax_style);
    this->setCompleter  (completer);
    this->setHighlighter(highlighter);

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    connect(this, SIGNAL(textChanged()),this, SLOT(onTextChanged()));
}

void CodeEditor::onTextChanged()
{
    qInfo("works!");
}

void CodeEditor::initData()
{

    completer = new QPythonCompleter(this);

    highlighter = new QPythonHighlighter;

    syntax_style = QSyntaxStyle::defaultStyle();

    // Loading styles
    loadStyle(":/styles/drakula.xml");
}

//QString CodeEditor::loadCode(QString path)
//{
//    QFile fl(path);

//    if (!fl.open(QIODevice::ReadOnly))
//    {
//        return QString();
//    }

//    return fl.readAll();
//}

void CodeEditor::loadStyle(QString path)
{
    QFile fl(path);

    if (!fl.open(QIODevice::ReadOnly))
    {
        return;
    }

    auto style = new QSyntaxStyle(this);

    if (!style->load(fl.readAll()))
    {
        delete style;
        return;
    }

    syntax_style = style;
}
