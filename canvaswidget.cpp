#include "canvaswidget.h"
#include "toolmode.h"
#include <QMouseEvent>
#include <cmath>


CanvasWidget:: CanvasWidget(QWidget *parent){
    m_image = QImage(QSize(1800, 1600), QImage::Format_ARGB32);
    m_image.fill(Qt::transparent);
    m_brushSize = 5;
    m_brushColor = Qt::black;

    QPainter p(&m_image);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.fillRect(m_image.rect(), Qt::white);
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

    painter.drawImage(0, 0, m_image); // (x, y, qimage object holding the raster data to render)


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
        QPainter painter(&m_image); // calling on the raster data in the canvas
        setupPainterForCurrentTool(painter);
        painter.drawPoint(m_lastPoint); // basically saying "draw a point here"
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

    QPainter painter(&m_image);
    // painter.drawPoint(m_lastPoint); // targets last point

    setupPainterForCurrentTool(painter);

    painter.setPen(Qt::NoPen);
    painter.setBrush(m_brushColor);
    // painter.drawEllipse(event->pos(), m_brushSize/2, m_brushSize/2);

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
    update();

}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (m_drawingShape) {
        // Commiting the shape to the image
        QPainter painter(&m_image);
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
        update();
    }
}
