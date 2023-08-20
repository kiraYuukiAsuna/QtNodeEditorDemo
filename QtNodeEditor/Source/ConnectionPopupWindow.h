#pragma once

#include <QDialog>
#include <Definitions.hpp>
#include <QVBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include "nlohmann_json/json.hpp"

template<typename RightWidgetName>
RightWidgetName* createProperty(QVBoxLayout* mainLayout, const QString& propertyName){
    auto* leftWidget = new QLabel(propertyName);
    auto* rightWidget = new RightWidgetName;
    auto* layout = new QHBoxLayout;
    layout->addWidget(leftWidget);
    layout->addWidget(rightWidget);
    mainLayout->addLayout(layout);
    return rightWidget;
}


class ConnectionPopupWindow : public QDialog {
    Q_OBJECT

public:
    ConnectionPopupWindow(QtNodes::ConnectionId connectionId, QVariant& conditionValue,QWidget* parent)
    : m_ConnectionId(connectionId), m_ConditionValue(conditionValue), QDialog(parent){
        constructView();
    }

private:
    void constructView(){
        if(!m_MainLayout){
            m_MainLayout = new QVBoxLayout;
        }

        nlohmann::json jsonObject;

        if(m_ConditionValue.canConvert<std::string>()){
            auto value = m_ConditionValue.value<std::string>();
            jsonObject = nlohmann::json::parse(value);
        }

        auto widget1 = createProperty<QDoubleSpinBox>(m_MainLayout, "CPU:");
        widget1->setValue(60.0);

        auto widget2 = createProperty<QDoubleSpinBox>(m_MainLayout, "Network IO:");
        widget2->setValue(50.0);

        auto widget3 = createProperty<QPushButton>(m_MainLayout, "Save Condition:");
        widget3->setText("Save");

        connect(widget3, &QPushButton::clicked, this, [this](bool checked=true){

        });

        jsonObject["cpu"] = 60.0;
        jsonObject["Network IO:"] = 50.0;

        m_ConditionValue = QVariant::fromValue(jsonObject.dump());

        this->setLayout(m_MainLayout);
    }



private:
    QtNodes::ConnectionId m_ConnectionId;
    QVariant& m_ConditionValue;

    QVBoxLayout* m_MainLayout{nullptr};
};

