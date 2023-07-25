#include "latexfileparser.h"

#include <QDebug>
#include <QString>
#include <QDir>

#include <string>
#include <vector>
#include <iostream>


LatexFileParser::LatexFileParser(PythonThread *pt)
    :QObject{nullptr},PythonUser{pt}
{

}

void LatexFileParser::parse(QString &filePath)
{
    QFile latexfile(filePath);
    QString latexstring;
    if (latexfile.open(QIODevice::ReadOnly| QIODevice::Text)) {
        latexstring = QString(latexfile.readAll());
        latexfile.close();
    }

    //ensure latexstring exists and does not contain ''' (python triple quotes)
    //can also get python to open the latex file! (rather than cpp)
    //put the following code into a resource python file prepending it with predefined variables from cpp (currentpath, latexstring, etc)


    PythonThread::Code code(QString(R"(
import sys
sys.path.append('%1\\python\\texsoup')
print(sys.path)
import TexSoup
soup = TexSoup.TexSoup(r''' %2 ''')
print(soup.find_all('includegraphics')[0].args[1])
varinpython='what a cool string'
    )").arg(QDir::currentPath(),latexstring), "latexfileparser",
        this, "recievePythonResult", "varinpython");

    pythonThread->queueCode(code);

}

void LatexFileParser::recievePythonResult(QSharedPointer<QString> result)
{
    qInfo() << "recieved result: "<< *result;

}


