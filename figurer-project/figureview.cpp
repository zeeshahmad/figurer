#include "figureview.h"

FigureView::FigureView(QWidget *parent): QLabel(parent)
{

}

void FigureView::showUpdatedFigure(QString &figureBase64)
{
    QByteArray ba = QByteArray::fromBase64(figureBase64.toUtf8());
    QImage image = QImage::fromData(ba, "PNG");
    setPixmap(QPixmap::fromImage(image));
    show();

    //working with code in editor:
    //figurer64="iVBORw0KGgoAAAANSUhEUgAAAAgAAAAIAQMAAAD+wSzIAAAABlBMVEX///+/v7+jQ3Y5AAAADklEQVQI12P4AIX8EAgALgAD/aNpbtEAAAAASUVORK5CYII"
}
