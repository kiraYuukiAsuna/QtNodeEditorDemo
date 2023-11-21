#pragma once

#include <QLineEdit>
#include <QtNodes/NodeDelegateModel>

#include <QDebug>
#include <QJsonArray>
#include "DefaultNodeData.h"
#include "FunctionJobInternalWidgetV1.h"

class FunctionDelegateModel : public QtNodes::NodeDelegateModel {
    Q_OBJECT
public:
    FunctionDelegateModel(){ }

    virtual ~FunctionDelegateModel() { }

    virtual bool captionVisible() const override { return true; }

    virtual QString caption() const override { return "Function";};

    virtual bool portCaptionVisible(QtNodes::PortType, QtNodes::PortIndex) const { return true; }

    virtual QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
        if (portType == QtNodes::PortType::In) {
            if(portIndex == 0){
                return "FuncIn";
            }
        }else if(portType == QtNodes::PortType::Out) {
            if(portIndex == 0){
                return "FuncOut";
            }
        }
        return "Invalid";
    }

    virtual QString name() const override { return "FunctionDelegateModel"; };

    QJsonObject save() const override{
        QJsonObject obj;
        QJsonArray array;

        auto exchange = m_InternalWidget->getInternalData();

        QJsonObject internalData;

        internalData["name"] = QString::fromStdString(exchange.name);
        internalData["times"] = QString::fromStdString(exchange.times);
        internalData["time"] = QString::fromStdString(exchange.time);
        internalData["outputType"] = QString::fromStdString(exchange.outputType);
        internalData["description"] = QString::fromStdString(exchange.description);

        obj["model-name"] = name();
        obj["data"] = internalData;
        return obj;
    }

    void load(QJsonObject const& obj) override{
        if(obj.contains("model-name")){
            auto name = obj["model-name"];
        }

        FunctionJobInternalWidgetV1DataExchange exchange;

        if(obj.contains("data")){
            auto data = obj["data"];
            if(data.isObject()){
                auto json = data.toObject();
                if(json.contains("name")){
                    exchange.name = json["name"].toString().toStdString();
                }
                if(json.contains("times")){
                    exchange.times = json["times"].toString().toStdString();
                }
                if(json.contains("time")){
                    exchange.time = json["time"].toString().toStdString();
                }
                if(json.contains("outputType")){
                    exchange.outputType = json["outputType"].toString().toStdString();
                }
                if(json.contains("description")){
                    exchange.description = json["description"].toString().toStdString();
                }
            }
        }

        m_InternalWidget->updateInternalData(exchange);
    }

    virtual unsigned int nPorts(QtNodes::PortType portType) const override{
        if (portType == QtNodes::PortType::In) {
            return 1;
        }else if(portType == QtNodes::PortType::Out) {
            return 1;
        }else{
            return 0;
        }
    }

    virtual QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override{
        if (portType == QtNodes::PortType::In) {
            if(portIndex == 0){
                return {.id="DefaultDataType",.name="DefaultDataType"};
            }
        }else if(portType == QtNodes::PortType::Out) {
            if(portIndex == 0){
                return {.id="DefaultDataType",.name="DefaultDataType"};
            }
        }
        return {.id="InvalidDataType",.name="InvalidDataType"};
    }

    virtual QtNodes::ConnectionPolicy portConnectionPolicy(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override{
        if (portType == QtNodes::PortType::In) {
            if(portIndex == 0){
                return QtNodes::ConnectionPolicy::One;
            }
        }else if(portType == QtNodes::PortType::Out) {
            if(portIndex == 0){
                return QtNodes::ConnectionPolicy::Many;
            }
        }
        return QtNodes::ConnectionPolicy::One;
    }

//    NodeStyle const &nodeStyle() const;
//
//    void setNodeStyle(NodeStyle const &style);

    virtual void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex const portIndex) override {
        qDebug()<< "Function In data recv";
    }

    virtual std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex const port) override {
        qDebug()<< "Function Out data";
        auto data = std::make_shared<DefaultNodeData>();
        return data;
    }

    virtual QWidget *embeddedWidget() override{
        if (!m_InternalWidget) {
            m_InternalWidget = new FunctionJobInternalWidgetV1{};

            FunctionJobInternalWidgetV1DataExchange exchange;

            exchange.name = "Name: test_function";
            exchange.times = "Times: 3";
            exchange.time = "Time: 30 min";
            exchange.outputType = "OutputType: multi";
            exchange.description = "This is a test function node!";

            m_InternalWidget->updateInternalData(exchange);
        }

        return m_InternalWidget;
    }

    virtual bool resizable() const { return false; }


    // slots
//    virtual void inputConnectionCreated(ConnectionId const &) {}
//
//    virtual void inputConnectionDeleted(ConnectionId const &) {}
//
//    virtual void outputConnectionCreated(ConnectionId const &) {}
//
//    virtual void outputConnectionDeleted(ConnectionId const &) {}

    // signals
//    /// Triggers the updates in the nodes downstream.
//    void dataUpdated(PortIndex const index);
//
//    /// Triggers the propagation of the empty data downstream.
//    void dataInvalidated(PortIndex const index);
//
//    void computingStarted();
//
//    void computingFinished();
//
//    void embeddedWidgetSizeUpdated();
//
//    /// Call this function before deleting the data associated with ports.
//    /**
//   * The function notifies the Graph Model and makes it remove and recompute the
//   * affected connection addresses.
//   */
//    void portsAboutToBeDeleted(PortType const portType, PortIndex const first, PortIndex const last);
//
//    /// Call this function when data and port moditications are finished.
//    void portsDeleted();
//
//    /// Call this function before inserting the data associated with ports.
//    /**
//   * The function notifies the Graph Model and makes it recompute the affected
//   * connection addresses.
//   */
//    void portsAboutToBeInserted(PortType const portType,
//                                PortIndex const first,
//                                PortIndex const last);
//
//    /// Call this function when data and port moditications are finished.
//    void portsInserted();

private:
    FunctionJobInternalWidgetV1* m_InternalWidget = nullptr;
};

