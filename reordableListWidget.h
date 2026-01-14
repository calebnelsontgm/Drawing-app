#ifndef REORDABLELISTWIDGET_H
#define REORDABLELISTWIDGET_H

#pragma once
#include <QListWidget>

class ReorderableListWidget : public QListWidget {
    Q_OBJECT
public:
    using QListWidget::QListWidget;

signals:
    void orderChanged();

protected:
    void dropEvent(QDropEvent *event) override {
        QListWidget::dropEvent(event);
        emit orderChanged();
    }
};


#endif // REORDABLELISTWIDGET_H
