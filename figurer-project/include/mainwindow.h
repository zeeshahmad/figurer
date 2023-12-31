#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTextEdit>
#include <QFile>

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
    StatusWidget* statusWidget;

    static void show_error(const QString& message);

Q_SIGNALS:
    void requestNewProject(QString&, QString&);
    void requestOpenProject(QString&);
    void requestCloseProject();

public Q_SLOTS:
    void updateEnabledStates(bool projectOpen);
    void updateFigureView(QSharedPointer<QByteArray> figureBase64);
private Q_SLOTS:
    void handleNewBtn();
    void handleOpenBtn();
    void handleCloseBtn();

private:
};
#endif // MAINWINDOW_H
