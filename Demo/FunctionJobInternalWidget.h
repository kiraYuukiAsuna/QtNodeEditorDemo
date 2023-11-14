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
#include <QGroupBox>
#include "CustomSelectWindow.h"
#include <QDebug>

struct FunctionJobCollectionInfo{
    std::string name;
    bool selected;
};

using FunctionJobCollectionInfoList = std::vector<FunctionJobCollectionInfo>;

enum class FunctionNodeType : int32_t {
    eNode,
    eFolder
};

struct FunctionInternalTreeNodeData{
    int32_t id;
    FunctionNodeType nodeType;
};
Q_DECLARE_METATYPE(FunctionInternalTreeNodeData)

// forward decl
class FunctionDelegateModel;

class FunctionJobInternalWidget : public QWidget {
Q_OBJECT

public:
    FunctionJobInternalWidget(FunctionDelegateModel* delegateModel, QWidget* parent= nullptr) : QWidget(parent) {
        m_DelegateModel = delegateModel;

        this->setPalette(QPalette(QColor(0xffffff)));

        m_MainLayout = new QVBoxLayout(this);

        m_FunctionJobView = new CustomSelectWidget({}, false, false, QIcon(ImageDefs::ImageEdit));
        m_ScriptsControlBtn = new QToolButton(this);

        m_ScriptsControlBtn->setToolTip(tr("Expand"));
        m_ExpandStatus = true;
        m_FunctionJobView->setVisible(m_ExpandStatus);

        {
            auto layout = new QHBoxLayout;

            m_ScriptsControlBtn->setIcon(QIcon(ImageDefs::ImageExpand));
            m_ScriptsControlBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);

            connect(m_ScriptsControlBtn, &QToolButton::clicked, this, [&](bool checked = false) {
                m_ExpandStatus = !m_ExpandStatus;
                m_FunctionJobView->setVisible(m_ExpandStatus);
                refreshView();
            });

            layout->addWidget(m_ScriptsControlBtn,222,Qt::AlignLeft);

            auto scriptsTool_EditBtn = new QToolButton;
            scriptsTool_EditBtn->setIcon(QIcon(ImageDefs::ImageEdit));
            scriptsTool_EditBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
            layout->addWidget(scriptsTool_EditBtn,222,Qt::AlignRight);
            connect(scriptsTool_EditBtn, &QToolButton::clicked, this, [&](bool checked = false) {

                CustomSelectWindow dialog({}, true, false, QIcon(ImageDefs::ImageEdit));

                for(auto& info : m_DataCache){
                    dialog.getCustomSelectedWidget()->addPath(info.name,true,info.selected);
                }

                if (dialog.exec() == QDialog::Accepted) {
                    std::vector<std::string> selectedPaths = dialog.getCustomSelectedWidget()->getSelectedPaths();

                    for(auto& path : selectedPaths){
                        for(auto& cache : m_DataCache) {
                            if (path == cache.name){
                                cache.selected = true;
                            }
                        }
                    }

                    refreshView();
                }

            });

            m_MainLayout->addLayout(layout);
        }

        m_MainLayout->addWidget(m_FunctionJobView);

        this->setLayout(m_MainLayout);

    }


    void updateCollection(FunctionJobCollectionInfoList& data){
        m_DataCache = std::move(data);
        refreshView();
    }

    FunctionJobCollectionInfoList getCollection(){
        return m_DataCache;
    }

    void refreshView(){
        std::vector<std::string> paths;

        for(auto& info : m_DataCache){
            if(info.selected){
                paths.push_back(info.name);
            }
        }

        m_FunctionJobView->clearAllAndUpdate(paths, false);

        if (!m_ExpandStatus) {
            m_ScriptsControlBtn->setIcon(QIcon(ImageDefs::ImageExpand));
            m_ScriptsControlBtn->setToolTip(tr("Expand"));
        } else {
            m_ScriptsControlBtn->setIcon(QIcon(ImageDefs::ImageCollapse));
            m_ScriptsControlBtn->setToolTip(tr("Collapse"));
        }

        qDebug()<<m_ExpandStatus;
        requstReCalcSize();
    }

    void requstReCalcSize();

private:
    QVBoxLayout* m_MainLayout;
    CustomSelectWidget* m_FunctionJobView;
    QToolButton* m_ScriptsControlBtn;

    FunctionJobCollectionInfoList m_DataCache;

    FunctionDelegateModel* m_DelegateModel;
    bool m_ExpandStatus;

};


