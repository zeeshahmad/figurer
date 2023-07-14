#ifndef FIGUREVIEW_H
#define FIGUREVIEW_H

#include <QGraphicsView>

class FigureView : public QGraphicsView
{
    Q_OBJECT
public:
    FigureView(QWidget *parent = 0);

private Q_SLOTS:

    void showUpdatedFigure(QString &figurebase64);
};

#endif // FIGUREVIEW_H
