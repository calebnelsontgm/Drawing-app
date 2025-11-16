#include "topbar.h"
#include <QDebug>

TopBar::TopBar(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(80); // Height of the Top Bar
    colorPreview = new QPushButton("Color", this);


    mySlider = new QSlider(Qt::Horizontal, this);
    mySlider->setFixedWidth(200);
    mySlider->setValue(5);
    myLabel = new QLabel(this);
    myLabel->setText(QString::number(mySlider->value()));


    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(mySlider);
    layout->addWidget(myLabel);
    layout->addWidget(colorPreview);

    connect(mySlider, &QSlider::valueChanged, this, &TopBar::onSliderValueChanged);
        // connect() is qt's built in function that sets up communication between objects
        // connect( Sender Object , Pointer to Signal Function, The current object, and then the pointer to the slot function )
        // sender objects send out a signal. in this case its the slider.
        // slot function is the function you want to call when the signal fires. "Call this specific function."

        // "Connect mySlider's valueChanged signal to this TopBar's onSliderValueChanged function, so when the slider value changes, that function gets called automatically."
    connect(colorPreview, &QPushButton::clicked, this, &TopBar::onColorButtonClicked);

}


void TopBar::onSliderValueChanged(int value){
    qDebug() << "Slider value:" << value;  // Prints to console
    myLabel->setText(QString::number(value));  // sends value to the slider displayer
    emit penWidthChanged(value);
}

void TopBar::onColorButtonClicked(bool checked){
    QColor selectedColor = QColorDialog::getColor(Qt::black, this, "Select Brush Color"); // pens the color picker dialog, starting with black as the default color
    if(selectedColor.isValid()){
        emit colorChanged(selectedColor);
    } // checks if the user picked an actual color instead of cancelling, and only emits the  signal if a color was picked.
}


/* void TopBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::red);  // Fill entire topbar with red
}
*/
