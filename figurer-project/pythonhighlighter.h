#ifndef PYTHONHIGHLIGHTER_H
#define PYTHONHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QStringLiteral>

class PythonHighlighter : public QSyntaxHighlighter
{
public:
    PythonHighlighter(QTextDocument*);

private:

    void highlightBlock(const QString&);

    struct HighlightRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QList<HighlightRule> highlightRules;

    //QRegularExpression commentStartExpresion;
    //QRegularExpression commentEndExpression;


    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

#endif // PYTHONHIGHLIGHTER_H
