#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include "toolmode.h"


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
    ToolMode m_currentTool;

public slots:
    void setBrushSize(int size);
    void setBrushColor(const QColor &color);
    void setToolMode(ToolMode mode);
    void setShapeType(ShapeType shape);

protected:
    void paintEvent(QPaintEvent *event) override; // QPaintEvent calls Qpainter. The *event is a pointer linked to another thing inherited from qpainter i believe.
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void setupPainterForCurrentTool(QPainter& painter);
    void mouseReleaseEvent(QMouseEvent *event);


private:
    QImage m_image; // Raster buffer holding the canvas pixels
    QPoint m_lastPoint;

    ShapeType m_currentShape;  // Circle, Square, etc.
    QPoint m_shapeStartPoint;  // Where user started dragging
    bool m_drawingShape = false;  // Track if currently drawing a shape

};

#endif // CANVASWIDGET_H
