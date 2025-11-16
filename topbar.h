#ifndef TOPBAR_H
#define TOPBAR_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QColorDialog>

class TopBar : public QWidget
{
    Q_OBJECT
public:
    TopBar(QWidget *parent = nullptr);
    QPushButton *myButton;
    QPushButton *colorPreview;
    QSlider *mySlider;
    QLabel *myLabel;
    QHBoxLayout *layout;


public slots:
    // void paintEvent(QPaintEvent *event) override;
    void onSliderValueChanged(int value);
    void onColorButtonClicked(bool checked);

signals:
    void penWidthChanged(int value);
    void colorChanged(QColor);

};

#endif // TOPBAR_H


