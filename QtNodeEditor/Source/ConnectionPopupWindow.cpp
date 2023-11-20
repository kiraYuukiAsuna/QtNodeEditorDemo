#include <QVBoxLayout>
#include "ConnectionPopupWindow.h"
#include "ui_ConnectionPopupWindow.h"

ConnectionPopupWindow::ConnectionPopupWindow(QJsonObject conditionDataJsonObject, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionPopupWindow), m_ConditionDataJsonObject(conditionDataJsonObject)
{
    ui->setupUi(this);

    setWindowTitle("Condition Settings");

    connect(ui->OkBtn, &QPushButton::clicked, this, [&](bool checked){
        accept();
    });

    connect(ui->CancelBtn, &QPushButton::clicked, this, [&](bool checked){
        reject();
    });

    readFromJsonObject();
}

ConnectionPopupWindow::~ConnectionPopupWindow()
{
    delete ui;
}

void ConnectionPopupWindow::readFromJsonObject() {
    Info info;

    if(m_ConditionDataJsonObject.contains("independent")){
        if(m_ConditionDataJsonObject["independent"].isBool()){
            info.independent = m_ConditionDataJsonObject["independent"].toBool(false);
        }
    }
    if(m_ConditionDataJsonObject.contains("allServerCPUHigherThan")){
        info.allServerCPUHigherThan = m_ConditionDataJsonObject["allServerCPUHigherThan"].toInt(0);
    }
    if(m_ConditionDataJsonObject.contains("anyServerCPUHigherThan")){
        info.anyServerCPUHigherThan = m_ConditionDataJsonObject["anyServerCPUHigherThan"].toInt(0);
    }
    if(m_ConditionDataJsonObject.contains("runPerformanceJobGroupEvenRun")){
        if(m_ConditionDataJsonObject["runPerformanceJobGroupEvenRun"].isBool()) {
            info.runPerformanceJobGroupInEvenRun = m_ConditionDataJsonObject["runPerformanceJobGroupEvenRun"].toBool(
                    false);
        }
    }
    if(m_ConditionDataJsonObject.contains("runPerformanceJobGroupAfter")){
        info.runPerformanceJobGroupAfter = m_ConditionDataJsonObject["runPerformanceJobGroupAfter"].toInt(0);
    }

    ui->independent->setChecked(info.independent);
    ui->allServerCPUHigherThan->setValue(info.allServerCPUHigherThan);
    ui->anyServerCPUHigherThan->setValue(info.anyServerCPUHigherThan);
    ui->runPerformanceJobGroupInEvenRun->setChecked(info.runPerformanceJobGroupInEvenRun);
    ui->runPerformanceJobGroupAfter->setValue(info.runPerformanceJobGroupAfter);
}

void ConnectionPopupWindow::writeToJsonObject() {
    Info info;

    info.independent = ui->independent->isChecked();
    info.allServerCPUHigherThan = ui->allServerCPUHigherThan->value();
    info.anyServerCPUHigherThan = ui->anyServerCPUHigherThan->value();
    info.runPerformanceJobGroupInEvenRun = ui->runPerformanceJobGroupInEvenRun->isChecked();
    info.runPerformanceJobGroupAfter = ui->runPerformanceJobGroupAfter->value();

    m_ConditionDataJsonObject["independent"] = info.independent;
    m_ConditionDataJsonObject["allServerCPUHigherThan"] = info.allServerCPUHigherThan;
    m_ConditionDataJsonObject["anyServerCPUHigherThan"] = info.anyServerCPUHigherThan;
    m_ConditionDataJsonObject["runPerformanceJobGroupEvenRun"] = info.runPerformanceJobGroupInEvenRun;
    m_ConditionDataJsonObject["runPerformanceJobGroupAfter"] = info.runPerformanceJobGroupAfter;
}
