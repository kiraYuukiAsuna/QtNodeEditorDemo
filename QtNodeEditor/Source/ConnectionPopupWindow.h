#pragma once

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class ConnectionPopupWindow;
}

class ConnectionPopupWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionPopupWindow(QJsonObject conditionDataJsonObject, QWidget *parent = nullptr);
    ~ConnectionPopupWindow();

    void readFromJsonObject();

    void writeToJsonObject();

    QJsonObject save(){
        writeToJsonObject();
        return m_ConditionDataJsonObject;
    }

    struct Info{
        bool independent = false;
        int allServerCPUHigherThan = 0;
        int anyServerCPUHigherThan = 0;
        bool runPerformanceJobGroupInEvenRun = false;
        int runPerformanceJobGroupAfter = 0;
    };

private:
    Ui::ConnectionPopupWindow *ui;

    QJsonObject m_ConditionDataJsonObject;
};
