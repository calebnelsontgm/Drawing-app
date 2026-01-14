#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "canvaswidget.h"
#include "topbar.h"
#include "layerspanel.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

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

    // Create a container widget.
    // This will become the central widget for the QMainWindow.
    QWidget *container = new QWidget(this);

    // Root layout: horizontal split.
    // Left side: Layers panel.
    // Right side: Topbar + Canvas stacked vertically.
    QHBoxLayout *rootLayout = new QHBoxLayout(container);
    rootLayout->setContentsMargins(0, 0, 0, 0);  // No margins
    rootLayout->setSpacing(1);                   // Small spacing

    // Right-side layout (Topbar + Canvas).
    // We make a nested vertical layout so the topbar stays above the canvas.
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setContentsMargins(0, 0, 0, 0); // No margins
    rightLayout->setSpacing(1);                  // No/low spacing between widgets

    // Create and add topbar.
    TopBar *topbar = new TopBar(this);
    rightLayout->addWidget(topbar);

    // Create and add canvas.
    // Create an instance of CanvasWidget (custom drawing widget).
    // Pass 'this' as the parent to integrate it into the MainWindow's widget hierarchy.
    CanvasWidget *canvas = new CanvasWidget(this);
    rightLayout->addWidget(canvas);

    // Create layers panel (uses the CanvasWidget to read/switch layers).
    LayersPanel *layersPanel = new LayersPanel(canvas, this);
    layersPanel->setFixedWidth(220); // Optional: makes it a visible side panel

    // Add left panel and right layout into the root horizontal layout.
    rootLayout->addWidget(layersPanel);
    rootLayout->addLayout(rightLayout);

    // Set container as central widget.
    // QMainWindow shows whatever is set as the central widget.
    setCentralWidget(container); // QMainWindow takes ownership of this widget. [web:75]

    // ---------------------------------------------------------
    // Signals/slots connections (unchanged)
    connect(topbar, &TopBar::penWidthChanged, canvas, &CanvasWidget::setBrushSize);
    // Connecting topbar's "pen width changed" value to the set brush size value.
    // This is basically taking the slider value in topbar and relaying it to canvaswidget.
    connect(topbar, &TopBar::colorChanged, canvas, &CanvasWidget::setBrushColor);

    connect(topbar, &TopBar::toolChanged, canvas, &CanvasWidget::setToolMode);

    connect(topbar, &TopBar::shapeSelected, canvas, &CanvasWidget::setShapeType);
}

// Destructor for MainWindow class. Cleans up allocated resources.
// Frees memory used by the UI components to prevent leaks.
MainWindow::~MainWindow()
{
    delete ui;
}
