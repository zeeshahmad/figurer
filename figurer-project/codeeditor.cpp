#include "codeeditor.h"
#include <QDebug>

#include <Python.h>


#include <string>
#include <iostream>
#include <stdio.h>
#include <nlohmann/json.hpp>
#include <sstream>
#include <vector>


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

    Py_Initialize();
    PyRun_SimpleString("counter=0");
    //Py_Finalize();

}

void CodeEditor::onTextChanged()
{
    qInfo("text changed!");
    std::vector<std::string> code = {
        "print('counter')\r\n",
        "counter = 1\r\n"
    };

    std::stringstream codestream;
    std::string py_command_begin = "";
    std::string py_command_end = "";

    codestream << py_command_begin;
    for (std::string& codeline : code) {
        codestream << codeline;
    }
    codestream << py_command_end;
    std::cout << codestream.str() << std::endl;
    std::string codestring = codestream.str();
    const char* codechar = codestring.c_str();


Py_Initialize();
    PyRun_SimpleString(codechar);
Py_Finalize();
}

void CodeEditor::initData()
{

    completer = new QPythonCompleter(this);

    highlighter = new QPythonHighlighter;

    syntax_style = QSyntaxStyle::defaultStyle();

    // Loading styles
    loadStyle(":/styles/drakula.xml");
    //somethings changed
    qInfo("something changed");
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
