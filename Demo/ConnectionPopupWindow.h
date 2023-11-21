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
        bool independentChecked = false;

        bool allServerCPUHigherThanChecked = false;
        int allServerCPUHigherThanValue = 0;

        bool anyServerCPUHigherThaChecked = false;
        int anyServerCPUHigherThanValue = 0;

        bool runPerformanceJobGroupAfterChecked = false;
        int runPerformanceJobGroupAfterValue = 0;

        bool runPerformanceJobGroupInEvenRunChecked = false;
    };

private:
    Ui::ConnectionPopupWindow *ui;
};
