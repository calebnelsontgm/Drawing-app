#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include "toolmode.h"
#include <vector>
#include <QKeyEvent>

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

    struct LayerInfo { QString name; bool visible; };

    int layerCount() const;
    int activeLayerIndex() const;
    LayerInfo layerInfo(int index) const;
    void setLayerVisible(int index, bool visible);

public slots:

    void addLayer();                 // create new transparent layer on top
    void deleteActiveLayer();        // remove active layer (if > 1)
    void setActiveLayer(int index);  // switch active layer by index

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
    void keyPressEvent(QKeyEvent *event) override;


private:
    QImage m_image; // Raster buffer holding the canvas pixels
    QPoint m_lastPoint;

    ShapeType m_currentShape;  // Circle, Square, etc.
    QPoint m_shapeStartPoint;  // Where user started dragging
    bool m_drawingShape = false;  // Track if currently drawing a shape

    // --- NEW: layer components ---
    struct Layer {
        QImage image;          // actual pixels for this layer
        QString name;          // for UI later
        bool visible = true;   // toggle later
        qreal opacity = 1.0;   // 0..1 (optional now)
    };

    std::vector<Layer> m_layers; // layer stack bottom->top
    int m_activeLayer = 0;       // index into m_layers

    // Cached composite image (flattened result of visible layers)
    QImage m_composited;
    bool m_compositeDirty = true;

    // Document size (so every layer is same size)
    QSize m_docSize = QSize(1800, 1600);

    // Helpers (internal)
    void rebuildComposite();     // flatten m_layers -> m_composited
    QImage& activeLayerImage();  // returns m_layers[m_activeLayer].image
    
};

#endif // CANVASWIDGET_H
