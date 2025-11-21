#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "canvaswidget.h"
#include <QVBoxLayout>
#include "topbar.h"



// Constructor for MainWindow class. Called when the main window is created.
// Initializes the base QMainWindow with an optional parent widget.
// Allocates and initializes the user interface from the .ui XML description.
// Sets up all UI elements (buttons, layouts, widgets) as designed in Qt Designer.
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ---------------------------------------------------------

    // Create a container widget
    QWidget *container = new QWidget(this);

    // Create vertical layout
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);  // No margins
    layout->setSpacing(1);  // No spacing between widgets

    // Create and add topbar
    TopBar *topbar = new TopBar(this);
    layout->addWidget(topbar);

    // Create and add canvas
    // Create an instance of CanvasWidget (custom drawing widget).
    // Pass 'this' as the parent to integrate it into the MainWindow's widget hierarchy.
    CanvasWidget *canvas = new CanvasWidget(this);
    layout->addWidget(canvas);

    // Set container as central widget
    setCentralWidget(container);

    connect(topbar, &TopBar::penWidthChanged, canvas, &CanvasWidget::setBrushSize);
    // connecting topbar's "pen width changed" value to the set brush size value
    // this is basically taking the slider value in topbar and relaying it to canvaswidget
    connect(topbar, &TopBar::colorChanged, canvas, &CanvasWidget::setBrushColor);

    connect(topbar, &TopBar::toolChanged, canvas, &CanvasWidget::setToolMode);

}



// Destructor for MainWindow class. Cleans up allocated resources.
// Frees memory used by the UI components to prevent leaks.
MainWindow::~MainWindow()
{
    delete ui;
}
