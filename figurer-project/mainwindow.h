#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTextEdit>
#include <QFile>

#include "codeeditor.h"
#include "pythonworker.h"
#include "statuswidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;

Q_SIGNALS:

private Q_SLOTS:
    void handleOpenLatex();

private:
    QString latexstring;

    CodeEditor* codeEditor;
    PythonWorker* pythonWorker;
    StatusWidget* statusWidget;
};
#endif // MAINWINDOW_H
