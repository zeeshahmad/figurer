#include <QTest>
#include <QSignalSpy>
#include <QTemporaryFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>

#include "latexfileparser.h"

class LatexFileParserTest: public QObject
{
    Q_OBJECT;

private Q_SLOTS:
    void canParseCorrectFileExtension();
    void findAllIncludeGraphicsTagsInLatexFile();
    void emitErrorWhenLatexFileDoesntExist();
    void emitErrorWhenLatexError();
private:

};



void LatexFileParserTest::canParseCorrectFileExtension()
{
    pycode::Worker pt;
    LatexFileParser parser(&pt);

    bool goodResult = parser.canParse("path/file.TEX")
                    && parser.canParse("path/file.tEx")
                    && parser.canParse("path/file.tex")
                    && parser.canParse("some/arbitrary/path/file.tex");
    bool badResult = parser.canParse("path/file.json")
        || parser.canParse("path/filetex")
        || parser.canParse("path/file.doc")
        || parser.canParse("path/file.docx");


    QVERIFY2(goodResult, "parse a .tex extention");
    QVERIFY(badResult == false);
}

void LatexFileParserTest::findAllIncludeGraphicsTagsInLatexFile()
{
    QTemporaryFile latexfile("latexfileparser-xxx.tex");
    if (!latexfile.open()) QFAIL("Could not open a test latex file");
    latexfile.write(QString(R"(
        \begin{document}
        \includegraphics{figure.png}
        \includegraphics[]{figure.png}
        \includegraphics[width=2]{anotherfig.jpg}
        \includegraphics[scale=0.5 \widthText]{fig3.eps}
        \includegraphics{long/path/to/some/fig.pdf}
        \begin{figure}
            \includegraphics[]{figure.anyextension}
        \end{figure}
        \end{document}
    )").toUtf8());
    latexfile.close();

    pycode::Worker pt;
    LatexFileParser parser(&pt);
    pt.start();

    QJsonObject parsed = parser.parse(latexfile.fileName());
    QStringList listoffigures = parsed["includegraphics"].toVariant().toStringList();

    QVERIFY(listoffigures.contains("figure.png"));
    QVERIFY(listoffigures.contains("anotherfig.jpg"));
    QVERIFY(listoffigures.contains("fig3.eps"));
    QVERIFY(listoffigures.contains("long/path/to/some/fig.pdf"));
    QVERIFY(listoffigures.contains("figure.anyextension"));
    QCOMPARE(listoffigures.count(),5); //duplicates are ignored
}

void LatexFileParserTest::emitErrorWhenLatexFileDoesntExist()
{
    pycode::Worker pt;
    LatexFileParser parser(&pt);
    pt.start();

    QSignalSpy spy(&parser, SIGNAL(error_latexfilecouldnotopen(QString)));
    QJsonObject parsed = parser.parse("nonexistent.file");
    QString errortext = spy.first().first().toString();

    QCOMPARE(spy.count(),1);
    QCOMPARE(errortext, "Couldn't open latex file nonexistent.file for parsing.");
    QCOMPARE(parsed.keys().count(),0);
}


void LatexFileParserTest::emitErrorWhenLatexError()
{
    QTemporaryFile latexfile("latexfileparser-xxx.tex");
    if (!latexfile.open()) QFAIL("Could not open a test latex file");
    latexfile.write(QString(R"(
        \begin{document}
        \includegraphics{figure.png}
        \incorrectcommand{document}
    )").toUtf8());
    latexfile.close();

    pycode::Worker pt;
    LatexFileParser parser(&pt);
    pt.start();

    QSignalSpy spy(&parser, SIGNAL(error_latexerror(QString)));
    QJsonObject parsed = parser.parse(latexfile.fileName());
    QStringList listoffigures = parsed["includegraphics"].toVariant().toStringList();
    QString errortext = spy.first().first().toString();

    QCOMPARE(spy.count(),1);
    QVERIFY(errortext.startsWith("There was a problem parsing the latex file: EOFError:"));
    QVERIFY(!listoffigures.contains("figure.png"));
}



QTEST_MAIN(LatexFileParserTest)
#include "latexfileparser_test.moc"
