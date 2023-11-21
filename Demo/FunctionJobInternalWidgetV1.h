#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

struct FunctionJobInternalWidgetV1DataExchange{
    std::string name;
    std::string times;
    std::string time;
    std::string outputType;
    std::string description;
};

class FunctionJobInternalWidgetV1:public QWidget {
    Q_OBJECT

    public:
    FunctionJobInternalWidgetV1();

    void updateInternalData(FunctionJobInternalWidgetV1DataExchange& exchange);
    FunctionJobInternalWidgetV1DataExchange getInternalData();

private:
    QGridLayout m_MainLayout;

    QLabel m_Name;
    QLabel m_Times;
    QLabel m_Time;
    QLabel m_OutputType;
    QLabel m_Description;
};
