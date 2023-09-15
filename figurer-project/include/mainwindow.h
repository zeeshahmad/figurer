#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTextEdit>
#include <QFile>

#include "projectmanager.h"
#include "projecttools.h"
#include "pythonthread.h"
#include "statuswidget.h"
#include <pybind11/embed.h>
#include <memory>

namespace py=pybind11;

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

    static void show_error(const QString& message);

    PythonThread *pythonThread;


Q_SIGNALS:
    void requestNewProject(QString&, QString&);
    void requestOpenProject(QString&);
    void requestCloseProject();

private Q_SLOTS:
    void handleNewBtn();
    void handleOpenBtn();
    void handleCloseBtn();
    void updateEnabledStates();

private:
    ProjectTools *tools;
    ProjectManager *projectManager;

    StatusWidget* statusWidget;

    void pythonTestFunc(py::dict*);
};
#endif // MAINWINDOW_H
