#include "layerspanel.h"
#include "canvaswidget.h"
#include "reordableListWidget.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

LayersPanel::LayersPanel(CanvasWidget* canvas, QWidget* parent)
    : QWidget(parent), m_canvas(canvas)
{
    m_list = new QListWidget(this);
    m_add = new QPushButton("+", this);
    m_del = new QPushButton("-", this);

    m_list->setDragDropMode(QAbstractItemView::InternalMove);
    m_list->setDefaultDropAction(Qt::MoveAction);

    auto* btnRow = new QHBoxLayout();
    btnRow->addWidget(m_add);
    btnRow->addWidget(m_del);

    auto* layout = new QVBoxLayout(this);
    layout->addLayout(btnRow);
    layout->addWidget(m_list);

    rebuildList();

    // Select layer by clicking row
    connect(m_list, &QListWidget::currentRowChanged, this, [this](int row){
        if (row < 0) return;
        m_canvas->setActiveLayer(row);
    }); // currentRowChanged is the intended signal for this [web:57]

    // Toggle visibility by clicking checkbox
    connect(m_list, &QListWidget::itemChanged, this, [this](QListWidgetItem* item){
        int row = m_list->row(item);
        if (row < 0) return;

        bool visible = (item->checkState() == Qt::Checked);
        m_canvas->setLayerVisible(row, visible); // you'll add this
    }); // itemChanged + checkState is a standard pattern [web:56]

    connect(m_add, &QPushButton::clicked, this, [this](){
        m_canvas->addLayer();
        rebuildList();
        m_list->setCurrentRow(m_canvas->activeLayerIndex());
    });

    connect(m_del, &QPushButton::clicked, this, [this](){
        m_canvas->deleteActiveLayer();
        rebuildList();
        m_list->setCurrentRow(m_canvas->activeLayerIndex());
    });
}

void LayersPanel::rebuildList() {
    m_list->blockSignals(true);
    m_list->clear();

    const auto count = m_canvas->layerCount();
    for (int i = 0; i < count; ++i) {
        auto info = m_canvas->layerInfo(i); // name + visible
        auto* item = new QListWidgetItem(info.name, m_list);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(info.visible ? Qt::Checked : Qt::Unchecked);
    }

    m_list->setCurrentRow(m_canvas->activeLayerIndex());
    m_list->blockSignals(false);
}
