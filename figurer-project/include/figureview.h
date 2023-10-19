#ifndef FIGUREVIEW_H
#define FIGUREVIEW_H

#include <QLabel>

class FigureView : public QLabel
{
    Q_OBJECT
public:
    FigureView(QWidget *parent = 0);

public Q_SLOTS:

    void showUpdatedFigure(QSharedPointer<QByteArray> figureImageData);
};

#endif // FIGUREVIEW_H
