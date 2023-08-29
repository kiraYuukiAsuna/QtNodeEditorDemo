#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QPushButton>
#include <QLabel>
#include "ImageDefs.h"
#include <vector>
#include <QToolButton>


struct PerformanceJobCollectionInfo{
    std::string name;
    bool selected;
};

using PerformanceJobCollectionInfoList = std::vector<PerformanceJobCollectionInfo>;

enum class NodeType : int32_t {
    eNode,
    eFolder
};

struct InternalTreeNodeData{
    int32_t id;
    NodeType nodeType;
};
Q_DECLARE_METATYPE(InternalTreeNodeData)

// forward decl
class PerformanceDelegateModel;

class PerformanceJobInternalWidget : public QWidget {
    Q_OBJECT

public:
    PerformanceJobInternalWidget(PerformanceDelegateModel* delegateModel, QWidget* parent= nullptr) : QWidget(parent) {
        m_DelegateModel = delegateModel;
        m_ExpandStatus = false;
        this->setPalette(QPalette(QColor(0xffffff)));

        m_MainLayout = new QVBoxLayout(this);
        m_PerformanceJobCollection = new QTreeWidget(this);
        m_ScriptsControlBtn = new QToolButton(this);

        m_PerformanceJobCollection->setHeaderHidden(true);
        m_PerformanceJobCollection->setColumnCount(1);
        m_PerformanceJobCollection->setAnimated(true);
        m_PerformanceJobCollection->setItemsExpandable(true);
        m_PerformanceJobCollection->setFrameStyle(QFrame::NoFrame);
        m_PerformanceJobCollection->setSortingEnabled(true);

        m_ScriptsControlBtn->setToolTip(tr("Expand"));


        {
            auto layout = new QHBoxLayout;

            m_ScriptsControlBtn->setIcon(QIcon(ImageDefs::ImageExpand));
            m_ScriptsControlBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);

            connect(m_ScriptsControlBtn, &QToolButton::clicked, this, [&](bool checked = false) {
                m_ExpandStatus = !m_ExpandStatus;
                refreshView();
            });

            layout->addWidget(m_ScriptsControlBtn,222,Qt::AlignLeft);

            auto scriptsTool_EditBtn = new QToolButton;
            scriptsTool_EditBtn->setIcon(QIcon(ImageDefs::ImageEdit));
            scriptsTool_EditBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
            layout->addWidget(scriptsTool_EditBtn,222,Qt::AlignRight);
            connect(scriptsTool_EditBtn, &QToolButton::clicked, this, [&](bool checked = false) {
            });

            m_MainLayout->addLayout(layout);
        }

        {
            m_PerformanceJobCollection->clear();

            m_PerformanceJobCollection->setHeaderHidden(true);
            m_PerformanceJobCollection->setColumnCount(1);
            m_PerformanceJobCollection->setAnimated(true);
            m_PerformanceJobCollection->setItemsExpandable(true);
            m_PerformanceJobCollection->setFrameStyle(QFrame::NoFrame);
            m_PerformanceJobCollection->setSortingEnabled(true);

            m_MainLayout->addWidget(m_PerformanceJobCollection);
        }

        this->setLayout(m_MainLayout);

    }


    void updateCollection(PerformanceJobCollectionInfoList& data){
        m_DataCache = std::move(data);
        refreshView();
    }

    void refreshView(){
        m_PerformanceJobCollection->clear();
        auto* topItem = new QTreeWidgetItem;
        topItem->setText(0, "Info");
        m_PerformanceJobCollection->addTopLevelItem(topItem);

        m_PerformanceJobCollection->setSortingEnabled(false);
        auto count = m_PerformanceJobCollection->topLevelItemCount();
        auto dataList(m_DataCache);

        for (auto& info : m_DataCache) {
            auto* item = new QTreeWidgetItem;
            item->setText(0, QString::fromStdString(info.name));
            topItem->addChild(item);
        }

        if (!m_ExpandStatus) {
            m_ScriptsControlBtn->setIcon(QIcon(ImageDefs::ImageExpand));
            m_ScriptsControlBtn->setToolTip(tr("Expand"));
        } else {
            m_ScriptsControlBtn->setIcon(QIcon(ImageDefs::ImageCollapse));
            m_ScriptsControlBtn->setToolTip(tr("Collapse"));
        }
        topItem->setExpanded(m_ExpandStatus);
        qDebug()<<m_ExpandStatus;
        requstReCalcSize();
    }

    void requstReCalcSize();

    private:
    QVBoxLayout* m_MainLayout;
    QTreeWidget* m_PerformanceJobCollection;
    QToolButton* m_ScriptsControlBtn;

    PerformanceJobCollectionInfoList m_DataCache;

    PerformanceDelegateModel* m_DelegateModel;
    bool m_ExpandStatus;

};


