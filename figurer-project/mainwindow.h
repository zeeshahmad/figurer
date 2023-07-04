#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "codeeditor.h"
//#include "pythonhighlighter.h"
#include <QMainWindow>
#include <QTextEdit>
#include <QFile>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;


    CodeEditor* codeEditor;


};
#endif // MAINWINDOW_H
