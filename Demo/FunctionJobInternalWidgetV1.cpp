#include "FunctionJobInternalWidgetV1.h"

FunctionJobInternalWidgetV1::FunctionJobInternalWidgetV1() {
    m_Name.setStyleSheet("font-size:16px;font-weight:bold;color:dark;background-color:white;");
    m_Times.setStyleSheet("font-size:16px;font-weight:bold;color:dark;background-color:white;");
    m_Time.setStyleSheet("font-size:16px;font-weight:bold;color:dark;background-color:white;");
    m_OutputType.setStyleSheet("font-size:16px;font-weight:bold;color:dark;background-color:white;");
    m_Description.setStyleSheet("font-size:16px;font-weight:bold;color:dark;background-color:white;");

    m_Name.setMinimumHeight(64);
    m_Name.setAlignment(Qt::AlignCenter);
    m_Name.setWordWrap(true);
    m_Times.setMinimumHeight(64);
    m_Times.setAlignment(Qt::AlignCenter);
    m_Times.setWordWrap(true);
    m_Time.setMinimumHeight(64);
    m_Time.setAlignment(Qt::AlignCenter);
    m_Time.setWordWrap(true);
    m_OutputType.setMinimumHeight(64);
    m_OutputType.setAlignment(Qt::AlignCenter);
    m_OutputType.setWordWrap(true);
    m_Description.setMinimumHeight(128);
    m_Description.setAlignment(Qt::AlignCenter);
    m_Description.setWordWrap(true);

    m_MainLayout.addWidget(&m_Name,0,0);
    m_MainLayout.addWidget(&m_Times,0,1);
    m_MainLayout.addWidget(&m_Time,1,0);
    m_MainLayout.addWidget(&m_OutputType,1,1);
    m_MainLayout.addWidget(&m_Description,2,0,1,2);

    m_MainLayout.setContentsMargins(6,6,6,6);
    m_MainLayout.setSpacing(6);

    auto* frame = new QFrame;
    frame->setLayout(&m_MainLayout);
    frame->setContentsMargins(0,0,0,0);
    frame->setStyleSheet("background-color:rgb(23, 100, 161);border:none;border-radius:12px;");

    auto* lay = new QHBoxLayout;
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(frame);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setContentsMargins(0,0,0,0);
    this->setLayout(lay);
//    this->setStyleSheet("background-color:rgb(0, 0, 0);");
}

void FunctionJobInternalWidgetV1::updateInternalData(FunctionJobInternalWidgetV1DataExchange& exchange) {
    m_Name.setText(QString::fromStdString(exchange.name));
    m_Times.setText(QString::fromStdString(exchange.times));
    m_Time.setText(QString::fromStdString(exchange.time));
    m_OutputType.setText(QString::fromStdString(exchange.outputType));
    m_Description.setText(QString::fromStdString(exchange.description));
}

FunctionJobInternalWidgetV1DataExchange FunctionJobInternalWidgetV1::getInternalData() {
    FunctionJobInternalWidgetV1DataExchange exchange;
    exchange.name = m_Name.text().toStdString();
    exchange.times = m_Times.text().toStdString();
    exchange.time = m_Time.text().toStdString();
    exchange.outputType = m_OutputType.text().toStdString();
    exchange.description = m_Description.text().toStdString();
    return exchange;
}

