#include "canvaswidget.h"
#include <QMouseEvent>
#include <cmath>


CanvasWidget:: CanvasWidget(QWidget *parent){
    m_image = QImage(QSize(1800, 1600), QImage::Format_ARGB32);
    m_image.fill(Qt::white);
    m_brushSize = 5;
    m_brushColor = Qt::black;
}

void CanvasWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this); // "create a painter that draws on this widget".
    painter.drawImage(0, 0, m_image); // (x, y, qimage object holding the raster data to render)
}

void CanvasWidget::mousePressEvent(QMouseEvent *event) {
    m_lastPoint = event->pos(); // last point stored when the mouse is clicked.
    QPainter painter(&m_image); // calling on the raster data in the canvas
    painter.drawPoint(m_lastPoint); // basically saying "draw a point here"
    update();   // Updates the canvas
}

void CanvasWidget::setBrushSize(int size) {
    m_brushSize = size;
}
// set brush size is recieving the slider's value relayed from mainwindow.cpp

void CanvasWidget::setBrushColor(const QColor &color) {
    m_brushColor = color;
}


void CanvasWidget::mouseMoveEvent(QMouseEvent *event) {
    QPainter painter(&m_image);
    // painter.drawPoint(m_lastPoint); // targets last point

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
        painter.drawEllipse(QPoint(x, y), m_brushSize, m_brushSize);
    }


    // painter.setPen(QPen(Qt::black, m_brushSize));
    // painter.drawLine(m_lastPoint, event->pos());    // connects last point with new point
    m_lastPoint = event->pos();
    update();
}
