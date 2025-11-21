# Qt Drawing Application

A desktop painting application built with Qt/C++ as a full-stack development learning project. Inspired by professional drawing tools like Krita, this project aims to create a fully-featured drawing app for digital artists.

## About

This project started from a passion for digital art and a curiosity about how drawing applications work. After building simpler drawing apps, I wanted to create something more complete that could eventually match the quality of professional tools. I chose Qt because it powers industry-standard applications like Krita and offers robust GUI capabilities for desktop development.

## Features

- Custom drawing canvas with mouse-based freehand drawing
- Toolbar interface with adjustable brush controls
- Real-time brush size slider with visual feedback display
- Eraser tool that sets pixels to transparent
- Color picker with live preview button
- Checkerboard background for transparency indication
- Event-driven architecture using Qt's signal/slot system
- Persistent drawing buffer using QImage raster graphics

## Tech Stack

- **Language:** C++ (C++17)
- **Framework:** Qt 6.x/5.x (Widgets module)
- **Graphics:** QImage, QPainter, QPaintEvent
- **UI Components:** QSlider, QLabel, QHBoxLayout
- **Build System:** CMake
- **Architecture:** Custom widget inheritance, signal/slot communication

## Project Structure

```
qt-drawing-app/
├── Header Files/
│ ├── canvaswidget.h # Custom drawing canvas widget
│ ├── mainwindow.h # Main application window
| ├── topbar.h # Toolbar with brush controls
│ └── toolmode.h # Holds the ToolMode enum for selecting brush, eraser, and other tool states
├── Source Files/
│ ├── canvaswidget.cpp # Canvas drawing logic
│ ├── mainwindow.cpp # Window setup and layout
│ ├── topbar.cpp # Toolbar functionality
│ └── main.cpp # Application entry point
└── CMakeLists.txt # Build configuration
```


## In Development

Core features currently being implemented:

- [x] Color picker and palette system
- [ ] Shape tools (circle, rectangle, line)
- [ ] Fill/bucket tool
- [x] Checkerboard transparent background
- [x] Eraser functionality
- [ ] Adjustable brush types and styles
- [ ] Image import/export (PNG, JPG, BMP)
- [ ] Canvas resize and zoom controls

## Future Roadmap

Long-term features planned for this project:

- Layer system with opacity and blend modes
- Advanced brush engine with pressure sensitivity
- Transform tools (rotate, scale, skew)
- Selection tools (rectangular, lasso, magic wand)
- Comprehensive undo/redo system
- AI-powered style transformation features
- Custom keyboard shortcuts

## Getting Started

### Prerequisites
- Qt 6.x or Qt 5.x
- CMake 3.16+
- C++17 compatible compiler (MSVC, GCC, or Clang)

### Building

```
mkdir build
cd build
cmake ..
cmake --build .
```


### Running
Execute the generated binary from the build directory.

## Development Notes

This project is my introduction to full-stack desktop application development with Qt. Key learning areas include:

- Qt's widget system and custom painting with QPainter
- Signal/slot architecture for component communication
- Event-driven programming with mouse event overrides
- Layout management systems (QHBoxLayout)
- QImage vs direct widget painting techniques
- Parent-child memory management in Qt

## Status

**Active Development** - Core canvas and toolbar implemented. Building toward a complete drawing application.

---

**Last Updated:** November 2025
