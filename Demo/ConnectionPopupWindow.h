#pragma once

#include "QtNodes/ConnectionPopupWindowBase.h"

namespace Ui {
class ConnectionPopupWindow;
}

class ConnectionPopupWindow : public ConnectionPopupWindowBase
{
    Q_OBJECT

public:
    explicit ConnectionPopupWindow(QJsonObject conditionDataJsonObject, QWidget *parent = nullptr);
    ~ConnectionPopupWindow();

    void readFromInternalJsonObject() override;

    void writeToInternalJsonObject() override;

    struct Info{
        bool independent = false;
        int allServerCPUHigherThan = 0;
        int anyServerCPUHigherThan = 0;
        bool runPerformanceJobGroupInEvenRun = false;
        int runPerformanceJobGroupAfter = 0;
    };

private:
    Ui::ConnectionPopupWindow *ui;
};
