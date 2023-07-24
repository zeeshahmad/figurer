#ifndef FIGUREVIEW_H
#define FIGUREVIEW_H

#include <Qlabel>

class FigureView : public QLabel
{
    Q_OBJECT
public:
    FigureView(QWidget *parent = 0);

private Q_SLOTS:

    void showUpdatedFigure(QString &figurebase64);
};

#endif // FIGUREVIEW_H
