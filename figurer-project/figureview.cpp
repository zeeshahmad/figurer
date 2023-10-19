#include "figureview.h"

FigureView::FigureView(QWidget *parent): QLabel(parent)
{

}

void FigureView::showUpdatedFigure(QSharedPointer<QByteArray> figureImageData)
{
    QImage image = QImage::fromData(*figureImageData, "PNG");
    setPixmap(QPixmap::fromImage(image));
}

/*
example python:

import matplotlib.pyplot as plt
fig, ax = plt.subplots()
ax.plot([1, 2, 3, 4], [14, 5, 20, 8])

figurer(fig)

*/
