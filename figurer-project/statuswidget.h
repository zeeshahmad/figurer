#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QWidget>
#include <QProgressBar>

class StatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StatusWidget(QWidget *parent = nullptr);
Q_SIGNALS:
    void cooldownCompleted(QString *);
public Q_SLOTS:
    void restartCooldown(QString *);
    void updateBar();
private:
    QProgressBar *cooldownBar;
    QTimer *timer;
    QString * pythonCode;
};

#endif // STATUSWIDGET_H
