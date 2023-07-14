#include "codeeditor.h"
//#include "./ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QLabel>

#include <QIODevice>

#include <nlohmann/json.hpp>


CodeEditor::CodeEditor(QWidget *parent): QCodeEditor(parent)
{
    qInfo("constructor!");
    initData();


    this->setSyntaxStyle(syntax_style);
    this->setCompleter(completer);
    this->setHighlighter(highlighter);

    connect(this, SIGNAL(textChanged()),this, SLOT(onTextChanged()));
//    ui->statusLabel->setText(QString("running python"));
}

void CodeEditor::onTextChanged()
{
    pythonCode = QString(this->toPlainText());
    Q_EMIT codeChanged(&pythonCode);
}

void CodeEditor::initData()
{
    completer = new QPythonCompleter(this);
    highlighter = new QPythonHighlighter;
    syntax_style = QSyntaxStyle::defaultStyle();

    // Loading styles
    loadStyle(":/styles/drakula.xml");
}



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
