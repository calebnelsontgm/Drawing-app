#ifndef TOPBAR_H
#define TOPBAR_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QColorDialog>
#include "toolmode.h"

class TopBar : public QWidget
{
    Q_OBJECT
public:
    TopBar(QWidget *parent = nullptr);
    QPushButton *myButton;
    QPushButton *colorPreview;
    QPushButton *eraserButton;
    QPushButton *brushButton;
    QPushButton *shapeButton;
    QSlider *mySlider;
    QLabel *myLabel;
    QHBoxLayout *layout;


public slots:
    // void paintEvent(QPaintEvent *event) override;
    void onSliderValueChanged(int value);
    void onColorButtonClicked(bool checked);
    void onBrushButtonClicked(bool checked);
    void onEraserButtonClicked(bool checked);
    void onShapeButtonClicked(bool checked);

signals:
    void penWidthChanged(int value);
    void colorChanged(QColor);
    void toolChanged(ToolMode mode);
    void shapeSelected(ShapeType shape);

};

#endif // TOPBAR_H


