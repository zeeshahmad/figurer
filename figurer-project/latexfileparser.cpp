#include "latexfileparser.h"

#include <QDebug>
#include <QString>
#include <QDir>

#include <string>
#include <vector>
#include <iostream>


LatexFileParser::LatexFileParser(PythonThread *pt)
    :PythonUser{pt}
{

}

void LatexFileParser::parse(QString &filePath)
{
    QFile latexfile(filePath);
    QString latexstring;
    if (latexfile.open(QIODevice::ReadOnly| QIODevice::Text)) {
        //QTextStream in(&latexfile);
        //QString latextext = in.readAll();
        latexstring = QString(latexfile.readAll());
        qInfo() << latexstring;
        latexfile.close();
    }

    //ensure latexstring exists and does not contain ''' (python triple quotes)
    //can also get python to open the latex file! (rather than cpp)
    //put the following code into a resource python file prepending it with predefined variables from cpp (currentpath, latexstring, etc)
    std::vector<std::string> code = {
        "import sys\n",
        "sys.path.append('",QDir::currentPath().toStdString(),"\\\\python\\\\texsoup')\n",
        "print(sys.path)\n",
        "import TexSoup\n",
        "soup = TexSoup.TexSoup(r'''",latexstring.toStdString(),"''')\n",
        "print(soup.find_all('includegraphics')[0].args[1])"
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
    //    std::cout << QDir::currentPath().toStdString() << std::endl;
    std::string codestring = codestream.str();
    const char* codechar = codestring.c_str();
}
