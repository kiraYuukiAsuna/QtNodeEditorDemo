#include "PerformanceJobInternalWidgetV1.h"
#include <QApplication>

PerformanceJobInternalWidgetV1::PerformanceJobInternalWidgetV1() {
    m_Name.setStyleSheet("font-size:24px;font-weight:bold;color:dark;");
    m_Times.setStyleSheet("font-size:24px;font-weight:bold;color:dark;");
    m_Time.setStyleSheet("font-size:24px;font-weight:bold;color:dark;");
    m_OutputType.setStyleSheet("font-size:24px;font-weight:bold;color:dark;");

//    m_Name.setFixedSize(128,32);
//    m_Name.setAlignment(Qt::AlignCenter);
//    m_Times.setFixedSize(128,32);
//    m_Times.setAlignment(Qt::AlignCenter);
//    m_Time.setFixedSize(128,32);
//    m_Time.setAlignment(Qt::AlignCenter);
//    m_OutputType.setFixedSize(128,32);
//    m_OutputType.setAlignment(Qt::AlignCenter);

    m_MainLayout.addWidget(&m_Name,0,0);
    m_MainLayout.addWidget(&m_Times,0,2);
    m_MainLayout.addWidget(&m_Time,2,0);
    m_MainLayout.addWidget(&m_OutputType,2,2);

    auto hline1 = new QFrame{};
    hline1->setFrameShape(QFrame::HLine);
    hline1->setStyleSheet("color:red;border:8px solid red");
    m_MainLayout.addWidget(hline1,1,0,1,1);
    auto hline2 = new QFrame{};
    hline2->setFrameShape(QFrame::HLine);
    hline2->setStyleSheet("color:red;border:8px solid red");
    m_MainLayout.addWidget(hline2,1,1,1,1);
    auto hline3 = new QFrame{};
    hline3->setFrameShape(QFrame::HLine);
    hline3->setStyleSheet("color:red;border:8px solid red");
    m_MainLayout.addWidget(hline3,1,2,1,1);

    auto vline = new QFrame{};
    vline->setFrameShape(QFrame::VLine);
    vline->setStyleSheet("color:red;border:3px solid red");


//    this->setStyleSheet("background-color:red");

    this->setLayout(&m_MainLayout);
}

void PerformanceJobInternalWidgetV1::updateInternalData(PerformanceJobInternalWidgetV1DataExchange& exchange) {
    m_Name.setText(QString::fromStdString(exchange.name));
    m_Times.setText(QString::fromStdString(exchange.times));
    m_Time.setText(QString::fromStdString(exchange.time));
    m_OutputType.setText(QString::fromStdString(exchange.outputType));
}

PerformanceJobInternalWidgetV1DataExchange PerformanceJobInternalWidgetV1::getInternalData() {
    PerformanceJobInternalWidgetV1DataExchange exchange;
    exchange.name = m_Name.text().toStdString();
    exchange.times = m_Times.text().toStdString();
    exchange.time = m_Time.text().toStdString();
    exchange.outputType = m_OutputType.text().toStdString();
    return exchange;
}

