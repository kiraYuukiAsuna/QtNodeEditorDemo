#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

struct PerformanceJobInternalWidgetV1DataExchange{
    std::string name;
    std::string times;
    std::string time;
    std::string outputType;
};

class PerformanceJobInternalWidgetV1:public QWidget {
Q_OBJECT

public:
    PerformanceJobInternalWidgetV1();

    void updateInternalData(PerformanceJobInternalWidgetV1DataExchange& exchange);
    PerformanceJobInternalWidgetV1DataExchange getInternalData();

private:
    QGridLayout m_MainLayout;

    QLabel m_Name;
    QLabel m_Times;
    QLabel m_Time;
    QLabel m_OutputType;
};
