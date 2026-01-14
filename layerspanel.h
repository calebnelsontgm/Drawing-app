#ifndef LAYERSPANEL_H
#define LAYERSPANEL_H

#pragma once
#include <QWidget>

class QListWidget;
class QPushButton;
class CanvasWidget;

class LayersPanel : public QWidget {
    Q_OBJECT
public:
    explicit LayersPanel(CanvasWidget* canvas, QWidget* parent = nullptr);

private:
    CanvasWidget* m_canvas;
    QListWidget* m_list;
    QPushButton* m_add;
    QPushButton* m_del;

    void rebuildList(); // rebuild from canvas layer data
};


#endif // LAYERSPANEL_H
