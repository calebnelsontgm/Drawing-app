#include "canvaswidget.h"
#include "toolmode.h"
#include <QMouseEvent>
#include <cmath>



CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent) {
    m_docSize = QSize(1800, 1600);

    m_brushSize = 5;
    m_brushColor = Qt::black;
    m_currentTool = Brush;

    // Create base/background layer (white like your old m_image)
    Layer base;
    base.name = "Background";
    base.image = QImage(m_docSize, QImage::Format_ARGB32);
    base.image.fill(Qt::transparent);

    QPainter p(&base.image);
    p.setCompositionMode(QPainter::CompositionMode_Source); // replace pixels [web:34]
    p.fillRect(base.image.rect(), Qt::white);

    m_layers.clear();
    m_layers.push_back(std::move(base));
    m_activeLayer = 0;

    // Composite cache
    m_composited = QImage(m_docSize, QImage::Format_ARGB32);
    m_composited.fill(Qt::transparent);
    m_compositeDirty = true;

    // Keep m_image for the moment if you want, but it's now obsolete
    m_image = QImage(m_docSize, QImage::Format_ARGB32);
    m_image.fill(Qt::transparent);

    setFocusPolicy(Qt::StrongFocus);
}


void CanvasWidget::setShapeType(ShapeType shape) {
    m_currentShape = shape;
}

void CanvasWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this); // "create a painter that draws on this widget".


    // Checkerboard parameters
    const int tileSize = 16;  // Each square is 16x16 pixels
    QColor lightColor(220, 220, 220); // Light gray
    QColor darkColor(160, 160, 160);  // Dark gray

    // Storing width and height values for the loop
    int width = this->width();
    int height = this->height();

    // Loop through and draw tiles
    for (int y = 0; y < height; y += tileSize) {
        for (int x = 0; x < width; x += tileSize) {
            bool isLight = ((x/tileSize) + (y/tileSize)) % 2 == 0;
            QRect tileRect(x, y, tileSize, tileSize);
            painter.fillRect(tileRect, isLight ? lightColor : darkColor);
        }
    }

    if (m_compositeDirty) rebuildComposite();
    painter.drawImage(0, 0, m_composited);
    // (x, y, qimage object holding the raster data to render)


    if (m_drawingShape) {
        QPainter previewPainter(this);
        previewPainter.setPen(QPen(m_brushColor, m_brushSize));
        previewPainter.setBrush(Qt::NoBrush);

        QRect shapeRect(m_shapeStartPoint, m_lastPoint);

        if (m_currentShape == Circle) {
            previewPainter.drawEllipse(shapeRect);
        } else if (m_currentShape == Square) {
            previewPainter.drawRect(shapeRect);
        } else if (m_currentShape == Line) {
            previewPainter.drawLine(m_shapeStartPoint, m_lastPoint);
        }
    }

}

void CanvasWidget::mousePressEvent(QMouseEvent *event) {
    m_lastPoint = event->pos(); // last point stored when the mouse is clicked.

    if (m_currentTool == Shape) {
        m_shapeStartPoint = event->pos();
        m_drawingShape = true;
    } else {
        QPainter painter(&activeLayerImage());
        setupPainterForCurrentTool(painter);
        painter.drawPoint(m_lastPoint);
        m_compositeDirty = true;
    }
    update();
}

void CanvasWidget::setBrushSize(int size) {
    m_brushSize = size;
}
// set brush size is recieving the slider's value relayed from mainwindow.cpp

void CanvasWidget::setBrushColor(const QColor &color) {
    m_brushColor = color;
}

void CanvasWidget::setToolMode(ToolMode mode) {
    QPainter painter(&m_image);
    m_currentTool = mode;
    qDebug() << "the mode is: " << mode;
}

void CanvasWidget::setupPainterForCurrentTool(QPainter& painter) {
    if (m_currentTool == Eraser) {
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        // painter.setBrush(Qt::transparent); // not strictly needed, but clear intent
    } else if (m_currentTool == Brush) {
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.setBrush(m_brushColor);
    }
    painter.setPen(Qt::NoPen);
}




void CanvasWidget::mouseMoveEvent(QMouseEvent *event) {

    if (m_drawingShape) {
        // Just update() to show preview - don't draw to image yet
        m_lastPoint = event->pos();
        update();
        return; // exit early before brush logic initiates.
    }

    QPainter painter(&activeLayerImage());
    setupPainterForCurrentTool(painter);

    painter.setPen(Qt::NoPen);
    painter.setBrush(m_brushColor);
    // painter.drawEllipse(event->pos(), m_brushSize/2, m_brushSize/2);

    if (m_currentTool != Eraser) {
        painter.setBrush(m_brushColor);
    }
    painter.setPen(Qt::NoPen);

    int dx = event->pos().x() - m_lastPoint.x();
    int dy = event->pos().y() - m_lastPoint.y();
    double distance = sqrt(dx*dx + dy*dy);
    int steps = (int)distance;  // One step per pixel of distance

    for(int i = 0; i <= steps; i++){
        double t = (double)i / steps;  // Ratio from 0.0 to 1.0
        int x = m_lastPoint.x() + t * (event->pos().x() - m_lastPoint.x());
        int y = m_lastPoint.y() + t * (event->pos().y() - m_lastPoint.y());
        painter.drawEllipse(QPoint(x, y), m_brushSize / 2 , m_brushSize / 2);
    }

    // painter.setPen(QPen(Qt::black, m_brushSize));
    // painter.drawLine(m_lastPoint, event->pos());    // connects last point with new point
    m_lastPoint = event->pos();
    m_compositeDirty = true;
    update();

}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (m_drawingShape) {
        // Commiting the shape to the image
        QPainter painter(&activeLayerImage());
        painter.setPen(QPen(m_brushColor, m_brushSize));
        painter.setBrush(Qt::NoBrush);  // Or m_brushColor if you want filled shapes

        QRect shapeRect(m_shapeStartPoint, event->pos());

        if (m_currentShape == Circle) {
            painter.drawEllipse(shapeRect);
        } else if (m_currentShape == Square) {
            painter.drawRect(shapeRect);
        } else if (m_currentShape == Line) {
            painter.drawLine(m_shapeStartPoint, event->pos());
        }
        m_drawingShape = false;
        m_compositeDirty = true;
        update();
    }
}

// === Layers ===

int CanvasWidget::layerCount() const { return int(m_layers.size()); }
int CanvasWidget::activeLayerIndex() const { return m_activeLayer; }

CanvasWidget::LayerInfo CanvasWidget::layerInfo(int index) const {
    return { m_layers[index].name, m_layers[index].visible };
}

void CanvasWidget::setLayerVisible(int index, bool visible) {
    if (index < 0 || index >= int(m_layers.size())) return;
    m_layers[index].visible = visible;
    m_compositeDirty = true;
    update();
}

// Return the QImage of the current layer
QImage& CanvasWidget::activeLayerImage() {
    return m_layers[m_activeLayer].image;
}

// Flatten m_layers into m_composited (bottom -> top)
void CanvasWidget::rebuildComposite() {
    if (m_composited.size() != m_docSize) {
        m_composited = QImage(m_docSize, QImage::Format_ARGB32);
    }
    m_composited.fill(Qt::transparent);

    QPainter p(&m_composited);
    p.setCompositionMode(QPainter::CompositionMode_SourceOver); // alpha blending [web:34]

    for (const Layer& layer : m_layers) {
        if (!layer.visible) continue;
        p.setOpacity(layer.opacity);
        p.drawImage(0, 0, layer.image);
    }

    p.setOpacity(1.0);
    m_compositeDirty = false;
}

// --- basic layer operations (no UI yet) ---
void CanvasWidget::addLayer() {
    Layer layer;
    layer.name = QString("Layer %1").arg(m_layers.size());
    layer.image = QImage(m_docSize, QImage::Format_ARGB32);
    layer.image.fill(Qt::transparent);

    m_layers.push_back(std::move(layer));
    m_activeLayer = int(m_layers.size()) - 1;

    m_compositeDirty = true;
    update();
}

void CanvasWidget::deleteActiveLayer() {
    if (m_layers.size() <= 1) return;
    m_layers.erase(m_layers.begin() + m_activeLayer);
    if (m_activeLayer >= int(m_layers.size()))
        m_activeLayer = int(m_layers.size()) - 1;

    m_compositeDirty = true;
    update();
}

void CanvasWidget::setActiveLayer(int index) {
    if (index < 0 || index >= int(m_layers.size())) return;
    m_activeLayer = index;
}

void CanvasWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_N:            // new layer
        addLayer();
        break;
    case Qt::Key_BracketLeft:  // previous layer
        setActiveLayer(std::max(0, m_activeLayer - 1));
        update();
        break;
    case Qt::Key_BracketRight: // next layer
        setActiveLayer(std::min(int(m_layers.size()) - 1, m_activeLayer + 1));
        update();
        break;
    case Qt::Key_V:            // toggle visibility
        m_layers[m_activeLayer].visible = !m_layers[m_activeLayer].visible;
        m_compositeDirty = true;
        update();
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }
}
