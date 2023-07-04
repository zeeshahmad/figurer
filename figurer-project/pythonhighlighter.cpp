#include "pythonhighlighter.h"

//https://wiki.python.org/moin/PyQt/Python%20syntax%20highlighting
//https://doc.qt.io/qt-6/qtwidgets-richtext-syntaxhighlighter-example.html

PythonHighlighter::PythonHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);

    const QString keywordPatterns[] = {
                                      QStringLiteral("\\band\\b"),QStringLiteral("\\bassert\\b"),QStringLiteral("\\bbreak\\b"),QStringLiteral("\\bclass\\b"),QStringLiteral("\\bcontinue\\b"),QStringLiteral("\\bdef\\b"),
                                      QStringLiteral("\\bdel\\b"),QStringLiteral("\\belif\\b"),QStringLiteral("\\belse\\b"),QStringLiteral("\\bexcept\\b"),QStringLiteral("\\bexec\\b"),QStringLiteral("\\bfinally\\b"),
                                      QStringLiteral("\\bfor\\b"),QStringLiteral("\\bfrom\\b"),QStringLiteral("\\bglobal\\b"),QStringLiteral("\\bif\\b"),QStringLiteral("\\bimport\\b"),QStringLiteral("\\bin\\b"),
                                      QStringLiteral("\\bis\\b"),QStringLiteral("\\blambda\\b"),QStringLiteral("\\bnot\\b"),QStringLiteral("\\bor\\b"),QStringLiteral("\\bpass\\b"),QStringLiteral("\\bprint\\b"),
                                      QStringLiteral("\\braise\\b"),QStringLiteral("\\breturn\\b"),QStringLiteral("\\btry\\b"),QStringLiteral("\\bwhile\\b"),QStringLiteral("\\byield\\b"),QStringLiteral("\\bNone\\b"),
                                      QStringLiteral("\\bTrue\\b"),QStringLiteral("\\bFalse\\b")
    };

    for (const QString &pattern: keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightRules.append(rule);
    }

    quotationFormat.setFontWeight(QFont::Bold);
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\"[^\"\\\\]*(\\.[^\"\\\\]*)*\""));
    rule.format = quotationFormat;
    highlightRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("'[^'\\\\]*(\\.[^'\\\\]*)*'"));
    highlightRules.append(rule);


    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::darkGray);
    singleLineCommentFormat.setFontItalic(true);
    rule.pattern = QRegularExpression(QStringLiteral("#[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightRules.append(rule);

}

void PythonHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightRule &rule : std::as_const(highlightRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
