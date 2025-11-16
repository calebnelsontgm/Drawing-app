#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QImage>


// Public: signals, constructors, interface.
// Protected: event handlers, helper logic.
// Private: internal data/state.

class CanvasWidget : public QWidget
{
    Q_OBJECT // Macro in class body for signal / event support
public:
    CanvasWidget(QWidget *parent = nullptr); // Optional parent. Lets parent-child memory management work. Required by most Qt tools and forms.
    int m_brushSize;
    QColor m_brushColor;

public slots:
    void setBrushSize(int size);
    void setBrushColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override; // QPaintEvent calls Qpainter. The *event is a pointer linked to another thing inherited from qpainter i believe.
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QImage m_image; // Raster buffer holding the canvas pixels
    QPoint m_lastPoint;
};

#endif // CANVASWIDGET_H
