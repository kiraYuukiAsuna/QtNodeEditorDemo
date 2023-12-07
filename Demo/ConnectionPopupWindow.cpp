#include <QVBoxLayout>
#include <utility>
#include "ConnectionPopupWindow.h"
#include "ui_ConnectionPopupWindow.h"

ConnectionPopupWindow::ConnectionPopupWindow(QJsonObject conditionDataJsonObject, QWidget *parent) :
    ui(new Ui::ConnectionPopupWindow),
    ConnectionPopupWindowBase(conditionDataJsonObject, parent)
{
    ui->setupUi(this);

    setWindowTitle("Condition Settings");

    connect(ui->OkBtn, &QPushButton::clicked, this, [&](bool checked){
        writeToInternalJsonObject();
        accept();
    });

    connect(ui->CancelBtn, &QPushButton::clicked, this, [&](bool checked){
        reject();
    });

    readFromInternalJsonObject();
}

ConnectionPopupWindow::~ConnectionPopupWindow()
{
    delete ui;
}

void ConnectionPopupWindow::readFromInternalJsonObject() {
    auto& jsonObject = getJsonObject();
    
    if(jsonObject.contains("independentChecked")){
        if(jsonObject["independentChecked"].isBool()){
            m_Info.independentChecked = jsonObject["independentChecked"].toBool(false);
        }
    }

    if(jsonObject.contains("allServerCPUHigherThanChecked")){
        if(jsonObject["allServerCPUHigherThanChecked"].isBool()){
            m_Info.allServerCPUHigherThanChecked = jsonObject["allServerCPUHigherThanChecked"].toBool(false);
        }
    }
    if(jsonObject.contains("allServerCPUHigherThanValue")){
        m_Info.allServerCPUHigherThanValue= jsonObject["allServerCPUHigherThanValue"].toInt(0);
    }

    if(jsonObject.contains("anyServerCPUHigherThanChecked")){
        m_Info.anyServerCPUHigherThaChecked = jsonObject["anyServerCPUHigherThanChecked"].toBool(false);
    }
    if(jsonObject.contains("anyServerCPUHigherThanValue")){
        m_Info.anyServerCPUHigherThanValue = jsonObject["anyServerCPUHigherThanValue"].toInt(0);
    }

    if(jsonObject.contains("runPerformanceJobGroupAfterChecked")){
        if(jsonObject["runPerformanceJobGroupAfterChecked"].isBool()) {
            m_Info.runPerformanceJobGroupAfterChecked = jsonObject["runPerformanceJobGroupAfterChecked"].toBool(false);
        }
    }
    if(jsonObject.contains("runPerformanceJobGroupAfterValue")){
        m_Info.runPerformanceJobGroupAfterValue = jsonObject["runPerformanceJobGroupAfterValue"].toInt(0);
    }

    if(jsonObject.contains("runPerformanceJobGroupEvenRunChecked")){
        if(jsonObject["runPerformanceJobGroupEvenRunChecked"].isBool()) {
            m_Info.runPerformanceJobGroupInEvenRunChecked = jsonObject["runPerformanceJobGroupEvenRunChecked"].toBool(
                    false);
        }
    }

    ui->independent->setChecked(m_Info.independentChecked);
    ui->allServerCPUHigherThan->setChecked(m_Info.allServerCPUHigherThanChecked);
    ui->allServerCPUHigherThanValue->setValue(m_Info.allServerCPUHigherThanValue);
    ui->anyServerCPUHigherThan->setChecked(m_Info.anyServerCPUHigherThaChecked);
    ui->anyServerCPUHigherThanValue->setValue(m_Info.anyServerCPUHigherThanValue);
    ui->runPerformanceJobGroupAfter->setChecked(m_Info.runPerformanceJobGroupAfterChecked);
    ui->runPerformanceJobGroupAfterValue->setValue(m_Info.runPerformanceJobGroupAfterValue);
    ui->runPerformanceJobGroupInEvenRun->setChecked(m_Info.runPerformanceJobGroupInEvenRunChecked);
}

void ConnectionPopupWindow::writeToInternalJsonObject() {
    auto& jsonObject = getJsonObject();

    m_Info.independentChecked = ui->independent->isChecked();
    m_Info.allServerCPUHigherThanChecked = ui->allServerCPUHigherThan->isChecked();
    m_Info.allServerCPUHigherThanValue = ui->allServerCPUHigherThanValue->value();
    m_Info.anyServerCPUHigherThaChecked = ui->anyServerCPUHigherThan->isChecked();
    m_Info.anyServerCPUHigherThanValue = ui->anyServerCPUHigherThanValue->value();
    m_Info.runPerformanceJobGroupAfterChecked = ui->runPerformanceJobGroupAfter->isChecked();
    m_Info.runPerformanceJobGroupAfterValue = ui->runPerformanceJobGroupAfterValue->value();
    m_Info.runPerformanceJobGroupInEvenRunChecked = ui->runPerformanceJobGroupInEvenRun->isChecked();

    jsonObject.insert("independentChecked", m_Info.independentChecked);

    jsonObject.insert("allServerCPUHigherThanChecked", m_Info.allServerCPUHigherThanChecked);
    jsonObject.insert("allServerCPUHigherThanValue", m_Info.allServerCPUHigherThanValue);

    jsonObject.insert("anyServerCPUHigherThanChecked", m_Info.anyServerCPUHigherThaChecked);
    jsonObject.insert("anyServerCPUHigherThanValue", m_Info.anyServerCPUHigherThanValue);

    jsonObject.insert("runPerformanceJobGroupAfterChecked", m_Info.runPerformanceJobGroupAfterChecked);
    jsonObject.insert("runPerformanceJobGroupAfterValue", m_Info.runPerformanceJobGroupAfterValue);

    jsonObject.insert("runPerformanceJobGroupEvenRunChecked", m_Info.runPerformanceJobGroupInEvenRunChecked);
}
