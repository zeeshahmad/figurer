#include "codeeditor.h"
#include <QDebug>
#include <QDir>
#include <QLabel>
#include <QIODevice>

CodeEditor::CodeEditor(QWidget *parent): QCodeEditor(parent)
{
    initData();

    this->setSyntaxStyle(syntax_style);
    this->setCompleter(completer);
    this->setHighlighter(highlighter);

    connect(this, SIGNAL(textChanged()),this, SLOT(onTextChanged()));
}

void CodeEditor::overwriteBuffers(const BufferData &newData)
{
    BufferDataIterator it(newData);
    while (it.hasNext()) {
        it.next();
        buffers[it.key()] = it.value();
    }
    if (visibleBufferExists()) {
        this->setText(visibleBufferText());
    }
}

void CodeEditor::showBuffer(const QString &bufferId)
{
    visibleBufferId = bufferId;
    this->setText(buffers[bufferId]);
}

QStringList CodeEditor::getBufferIds()
{
    return buffers.keys();
}

const QString CodeEditor::getBufferText(const QString &bufferId) const
{
    return buffers[bufferId];
}

void CodeEditor::onTextChanged()
{
    if (visibleBufferExists()) {
        visibleBufferText() = QString(this->toPlainText());
        if (bufferIsFigure(visibleBufferId)){ //this line is not unit tested
            Q_EMIT codeChanged(stitchTextForVisibleBuffer());
        }
    }
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

QString& CodeEditor::visibleBufferText()
{
    return buffers[visibleBufferId];
}

bool CodeEditor::visibleBufferExists()
{
    return !visibleBufferId.isEmpty();
}

void CodeEditor::visibleBufferToEmptyId()
{
    visibleBufferId = QString();
}

QString CodeEditor::stitchTextForBuffer(const QString &bufferId)
{
    //later move the functionality here to a different class
    QString finalText;
    finalText.append(buffers[":pre-python"]);
    finalText.append("\n");
    finalText.append(buffers[bufferId]);
    finalText.append("\n");
    finalText.append(buffers[":post-python"]);
    return finalText;
}

QString CodeEditor::stitchTextForVisibleBuffer()
{
    if (visibleBufferExists()) {
        return stitchTextForBuffer(visibleBufferId);
    }
    return QString();
}

bool CodeEditor::bufferIsFigure(const QString& bufferId)
{
    return !bufferId.startsWith(":");
}
