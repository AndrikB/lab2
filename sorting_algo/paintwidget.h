#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QColor>
#include <QPainter>

const QColor defaultLineColor = Qt::lightGray;
const QColor defaultBgColor = Qt::black;

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = nullptr);
    ~PaintWidget()override;
    void visualize(int size, int *array, int red=-1, int yelow=-1);

signals:
    void setFullScreen();

public slots:

private:
    QPen pen;


    int size;
    int* array=Q_NULLPTR;
    int red;
    int yelow;

    void paintEvent(QPaintEvent *event) override;

};

#endif // PAINTWIDGET_H
