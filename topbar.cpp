#include "topbar.h"
#include "toolmode.h"
#include <QDebug>
#include <QMenu>

TopBar::TopBar(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(80); // Height of the Top Bar


    // Button setup.
    colorPreview = new QPushButton(this);   // color selection
    colorPreview->setFixedSize(30, 30);
    colorPreview->setStyleSheet("background-color: black; border-radius: 15px;");

    eraserButton = new QPushButton("Eraser", this);   // eraser
    eraserButton->setFixedSize(60, 30);
    brushButton = new QPushButton("Brush", this);    // brush
    brushButton->setFixedSize(60, 30);
    shapeButton = new QPushButton("Shape", this);    // shape
    shapeButton->setFixedSize(60, 30);


    // mySlider setup.
    mySlider = new QSlider(Qt::Horizontal, this);
    mySlider->setFixedWidth(200);
    mySlider->setValue(5);
    myLabel = new QLabel(this);
    myLabel->setText(QString::number(mySlider->value()));

    // Widgets for buttons.
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(mySlider);
    layout->addWidget(myLabel);
    layout->addWidget(shapeButton);
    layout->addWidget(eraserButton);
    layout->addWidget(brushButton);
    layout->addWidget(colorPreview);


    // Connections between buttons and their respective functions.
    connect(mySlider, &QSlider::valueChanged, this, &TopBar::onSliderValueChanged);
        // connect() is qt's built in function that sets up communication between objects
        // connect( Sender Object , Pointer to Signal Function, The current object, and then the pointer to the slot function )
        // sender objects send out a signal. in this case its the slider.
        // slot function is the function you want to call when the signal fires. "Call this specific function."

        // "Connect mySlider's valueChanged signal to this TopBar's onSliderValueChanged function, so when the slider value changes, that function gets called automatically."
    connect(colorPreview, &QPushButton::clicked, this, &TopBar::onColorButtonClicked);
    connect(brushButton, &QPushButton::clicked, this, &TopBar::onBrushButtonClicked);
    connect(eraserButton, &QPushButton::clicked, this, &TopBar::onEraserButtonClicked);
    connect(shapeButton, &QPushButton::clicked, this, &TopBar::onShapeButtonClicked);

}


void TopBar::onSliderValueChanged(int value){
    // qDebug() << "Slider value:" << value;  // Prints to console
    myLabel->setText(QString::number(value));  // sends value to the slider displayer
    emit penWidthChanged(value);
}

void TopBar::onShapeButtonClicked(bool checked){ 
    QMenu* shapeMenu = new QMenu(this);
    QAction* circleAction = shapeMenu->addAction("Circle");
    QAction* squareAction = shapeMenu->addAction("Square");


    connect(circleAction, &QAction::triggered, this, [this](){
        emit toolChanged(Shape);
        emit shapeSelected(Circle);
        shapeButton->setText("Circle");  // Update button text
        qDebug() << "circle";
    });

    connect(squareAction, &QAction::triggered, this, [this]() {
        emit toolChanged(Shape);
        emit shapeSelected(Square);
        shapeButton->setText("Square");
        qDebug() << "square";
    });

    shapeButton->setMenu(shapeMenu);

}


void TopBar::onColorButtonClicked(bool checked){
    QColor selectedColor = QColorDialog::getColor(Qt::black, this, "Select Brush Color"); // pens the color picker dialog, starting with black as the default color

    // Checks if the user picked an actual color instead of cancelling, and only emits the  signal if a color was picked.
    if(selectedColor.isValid()){
        // Updating background color of the button
        QString styleSheet = QString("background-color: %1; border-radius: 15px;").arg(selectedColor.name());
        colorPreview->setStyleSheet(styleSheet);
        emit colorChanged(selectedColor);
    }
}

void TopBar::onBrushButtonClicked(bool checked){
    qDebug() << "brush clicked";
    emit toolChanged(Brush);
}

void TopBar::onEraserButtonClicked(bool checked){
    qDebug() << "eraser clicked";
    emit toolChanged(Eraser);
}


