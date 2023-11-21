#include <QVBoxLayout>
#include <utility>
#include "ConnectionPopupWindow.h"
#include "ui_ConnectionPopupWindow.h"

ConnectionPopupWindow::ConnectionPopupWindow(QJsonObject conditionDataJsonObject, QWidget *parent) :
    ui(new Ui::ConnectionPopupWindow),
    ConnectionPopupWindowBase(std::move(conditionDataJsonObject), parent)
{
    ui->setupUi(this);

    setWindowTitle("Condition Settings");

    connect(ui->OkBtn, &QPushButton::clicked, this, [&](bool checked){
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
    Info info;

    if(getJsonObject().contains("independentChecked")){
        if(getJsonObject()["independentChecked"].isBool()){
            info.independentChecked = getJsonObject()["independentChecked"].toBool(false);
        }
    }

    if(getJsonObject().contains("allServerCPUHigherThanChecked")){
        if(getJsonObject()["allServerCPUHigherThanChecked"].isBool()){
            info.allServerCPUHigherThanChecked = getJsonObject()["allServerCPUHigherThanChecked"].toBool(false);
        }
    }
    if(getJsonObject().contains("allServerCPUHigherThanValue")){
        if(getJsonObject()["allServerCPUHigherThanValue"].isBool()){
            info.allServerCPUHigherThanValue= getJsonObject()["allServerCPUHigherThanValue"].toInt(0);
        }
    }


    if(getJsonObject().contains("anyServerCPUHigherThaChecked")){
        info.anyServerCPUHigherThaChecked = getJsonObject()["anyServerCPUHigherThaChecked"].toBool(false);
    }
    if(getJsonObject().contains("anyServerCPUHigherThanValue")){
        info.anyServerCPUHigherThanValue = getJsonObject()["anyServerCPUHigherThanValue"].toInt(0);
    }

    if(getJsonObject().contains("runPerformanceJobGroupAfterChecked")){
        info.runPerformanceJobGroupAfterChecked = getJsonObject()["runPerformanceJobGroupAfterChecked"].toBool(false);
    }
    if(getJsonObject().contains("runPerformanceJobGroupAfterValue")){
        info.runPerformanceJobGroupAfterValue = getJsonObject()["runPerformanceJobGroupAfterValue"].toInt(0);
    }

    if(getJsonObject().contains("runPerformanceJobGroupEvenRunChecked")){
        if(getJsonObject()["runPerformanceJobGroupEvenRunChecked"].isBool()) {
            info.runPerformanceJobGroupInEvenRunChecked = getJsonObject()["runPerformanceJobGroupEvenRunChecked"].toBool(
                    false);
        }
    }

    ui->independent->setChecked(info.independentChecked);
    ui->allServerCPUHigherThan->setChecked(info.allServerCPUHigherThanChecked);
    ui->allServerCPUHigherThanValue->setValue(info.allServerCPUHigherThanValue);
    ui->anyServerCPUHigherThan->setChecked(info.anyServerCPUHigherThaChecked);
    ui->anyServerCPUHigherThanValue->setValue(info.anyServerCPUHigherThanValue);
    ui->runPerformanceJobGroupAfter->setChecked(info.runPerformanceJobGroupAfterChecked);
    ui->runPerformanceJobGroupAfterValue->setValue(info.runPerformanceJobGroupAfterValue);
    ui->runPerformanceJobGroupInEvenRun->setChecked(info.runPerformanceJobGroupInEvenRunChecked);
}

void ConnectionPopupWindow::writeToInternalJsonObject() {
    Info info;

    getJsonObject()["independentChecked"] = info.independentChecked;

    getJsonObject()["allServerCPUHigherThanChecked"] = info.allServerCPUHigherThanChecked;
    getJsonObject()["allServerCPUHigherThanValue"] = info.allServerCPUHigherThanValue;

    getJsonObject()["anyServerCPUHigherThanChecked"] = info.anyServerCPUHigherThaChecked;
    getJsonObject()["anyServerCPUHigherThanValue"] = info.anyServerCPUHigherThanValue;

    getJsonObject()["runPerformanceJobGroupAfterChecked"] = info.runPerformanceJobGroupAfterChecked;
    getJsonObject()["runPerformanceJobGroupAfterValue"] = info.runPerformanceJobGroupAfterValue;

    getJsonObject()["runPerformanceJobGroupEvenRunChecked"] = info.runPerformanceJobGroupInEvenRunChecked;

}
