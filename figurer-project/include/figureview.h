#ifndef FIGUREVIEW_H
#define FIGUREVIEW_H

#include <QLabel>

class FigureView : public QLabel
{
    Q_OBJECT
public:
    FigureView(QWidget *parent = 0);

public Q_SLOTS:

    void showUpdatedFigure(QString &figureBase64);
};

#endif // FIGUREVIEW_H
