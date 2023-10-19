#include <QTest>
#include <QSignalSpy>
#include <QDebug>
#include <QBuffer>
#include <QImage>

#include "figureview.h"

class FigureViewTest: public QObject
{
    Q_OBJECT;

private Q_SLOTS:
    void showUpdatedFigureSlot();

private:
    QImage makeImage(int width, int height) {
        QImage img(width, height, QImage::Format_RGB32);

        for (int y=0;y<height;y++) {
            for (int x=0;x<width;x++) {
                img.setPixel(x,y, qRgb(x%256,y%256,(x+y)%256));
            }
        }
        return img;
    }
};


void FigureViewTest::showUpdatedFigureSlot()
{
    FigureView fview;
    QImage testImage = makeImage(10,10);

    QSharedPointer<QByteArray> barray = QSharedPointer<QByteArray>::create();
    QBuffer buffer(barray.get());
    buffer.open(QIODevice::WriteOnly);
    testImage.save(&buffer, "PNG");
    buffer.close();

    fview.showUpdatedFigure(barray);

    QCOMPARE(fview.pixmap().toImage(), testImage);
}

QTEST_MAIN(FigureViewTest)
#include "figureview_test.moc"
