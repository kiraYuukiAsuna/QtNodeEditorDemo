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

    if(getJsonObject().contains("independent")){
        if(getJsonObject()["independent"].isBool()){
            info.independent = getJsonObject()["independent"].toBool(false);
        }
    }
    if(getJsonObject().contains("allServerCPUHigherThan")){
        info.allServerCPUHigherThan = getJsonObject()["allServerCPUHigherThan"].toInt(0);
    }
    if(getJsonObject().contains("anyServerCPUHigherThan")){
        info.anyServerCPUHigherThan = getJsonObject()["anyServerCPUHigherThan"].toInt(0);
    }
    if(getJsonObject().contains("runPerformanceJobGroupEvenRun")){
        if(getJsonObject()["runPerformanceJobGroupEvenRun"].isBool()) {
            info.runPerformanceJobGroupInEvenRun = getJsonObject()["runPerformanceJobGroupEvenRun"].toBool(
                    false);
        }
    }
    if(getJsonObject().contains("runPerformanceJobGroupAfter")){
        info.runPerformanceJobGroupAfter = getJsonObject()["runPerformanceJobGroupAfter"].toInt(0);
    }

    ui->independent->setChecked(info.independent);
    ui->allServerCPUHigherThan->setValue(info.allServerCPUHigherThan);
    ui->anyServerCPUHigherThan->setValue(info.anyServerCPUHigherThan);
    ui->runPerformanceJobGroupInEvenRun->setChecked(info.runPerformanceJobGroupInEvenRun);
    ui->runPerformanceJobGroupAfter->setValue(info.runPerformanceJobGroupAfter);
}

void ConnectionPopupWindow::writeToInternalJsonObject() {
    Info info;

    info.independent = ui->independent->isChecked();
    info.allServerCPUHigherThan = ui->allServerCPUHigherThan->value();
    info.anyServerCPUHigherThan = ui->anyServerCPUHigherThan->value();
    info.runPerformanceJobGroupInEvenRun = ui->runPerformanceJobGroupInEvenRun->isChecked();
    info.runPerformanceJobGroupAfter = ui->runPerformanceJobGroupAfter->value();

    getJsonObject()["independent"] = info.independent;
    getJsonObject()["allServerCPUHigherThan"] = info.allServerCPUHigherThan;
    getJsonObject()["anyServerCPUHigherThan"] = info.anyServerCPUHigherThan;
    getJsonObject()["runPerformanceJobGroupEvenRun"] = info.runPerformanceJobGroupInEvenRun;
    getJsonObject()["runPerformanceJobGroupAfter"] = info.runPerformanceJobGroupAfter;
}
