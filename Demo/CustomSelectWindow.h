#pragma once

#include <QDialog>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include "CustomSelectWidget.h"

class CustomSelectWindow : public QDialog {
Q_OBJECT

public:
    CustomSelectWindow(const std::vector<std::string>& paths, bool withCheckBox, bool checkBoxStatus, QIcon icon, QWidget* parent = nullptr)
            : QDialog(parent) {
        setWindowTitle("Custom Dialog");

        auto* layout = new QVBoxLayout(this);

        m_CustomSelectWidget = new CustomSelectWidget(paths, withCheckBox, checkBoxStatus, icon, parent);

        auto* okButton = new QPushButton("OK", this);
        connect(okButton, &QPushButton::clicked, this, &CustomSelectWindow::accept);

        auto* cancelButton = new QPushButton("Cancel", this);
        connect(cancelButton, &QPushButton::clicked, this, &CustomSelectWindow::reject);

        layout->addWidget(m_CustomSelectWidget);

        auto* hlayout = new QHBoxLayout;

        hlayout->addWidget(okButton);
        hlayout->addWidget(cancelButton);

        layout->addLayout(hlayout);

    }

    CustomSelectWidget* getCustomSelectedWidget(){
    return m_CustomSelectWidget;
    }

    CustomSelectWidget* m_CustomSelectWidget;

};
