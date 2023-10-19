#include "statuswidget.h"
#include <QDebug>
#include <QProgressBar>
#include <QTimer>
#include <QString>

StatusWidget::StatusWidget(QWidget *parent)
    : QWidget{parent}
{
    cooldownBar = new QProgressBar(this);
    cooldownBar->setMaximum(50);
    cooldownBar->setValue(0);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &StatusWidget::updateBar);
}

void StatusWidget::restartCooldown(QString pythonCode)
{
    this->pythonCode = pythonCode;
    if (timer->isActive()) {
        timer->stop();
    }
    cooldownBar->setValue(0);
    timer->start(10);
}

void StatusWidget::updateBar()
{
    if (cooldownBar->value() < 50) {
        cooldownBar->setValue(cooldownBar->value()+1);
    } else {
        timer->stop();
        cooldownBar->setValue(0);
        Q_EMIT cooldownCompleted(pythonCode);
    }
}

