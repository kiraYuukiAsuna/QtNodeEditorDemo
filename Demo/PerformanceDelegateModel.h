#pragma once

#include <QLineEdit>
#include <QJsonArray>
#include "QtNodes/NodeDelegateModel"
#include "DefaultNodeData.h"
#include "PerformanceJobInternalWidget.h"

class PerformanceDelegateModel : public QtNodes::NodeDelegateModel  {
Q_OBJECT
public:
    PerformanceDelegateModel(){ }

    virtual ~PerformanceDelegateModel() { }

    virtual bool captionVisible() const override { return true; }

    virtual QString caption() const override { return "Performance";};

    virtual bool portCaptionVisible(QtNodes::PortType, QtNodes::PortIndex) const { return true; }

    virtual QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
        if (portType == QtNodes::PortType::In) {
            if(portIndex == 0){
                return "In";
            }
        }else if(portType == QtNodes::PortType::Out) {
            if(portIndex == 0){
                return "Out";
            }
        }
        return "Invalid";
    }

    virtual QString name() const override { return "PerformanceDelegateModel"; };

    QJsonObject save() const override{
        QJsonObject obj;
        QJsonArray array;

        auto data = m_InternalWidget->getCollection();
        for(auto& info : data){
            if(info.selected) {
                QJsonValue value;
                value = QJsonValue::fromVariant(QString::fromStdString(info.name));
                array.append(value);
            }
        }

        obj["model-name"] = name();
        obj["data"] = array;
        return obj;
    }

    void load(QJsonObject const& obj) override{
        if(obj.contains("model-name")){
            auto name = obj["model-name"];
        }

        PerformanceJobCollectionInfoList list;

        if(obj.contains("data")){
            auto data = obj["data"];
            if(data.isArray()){
                auto arrayData = data.toArray();

                for(QJsonValueRef value : arrayData){
                    if(value.isString()){
                        list.push_back({value.toString().toStdString(),true});
                    }
                }
            }
        }

        m_InternalWidget->updateCollection(list);
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
                return QtNodes::ConnectionPolicy::One;
            }
        }
        return QtNodes::ConnectionPolicy::One;
    }

//    NodeStyle const &nodeStyle() const;
//
//    void setNodeStyle(NodeStyle const &style);

    virtual void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex const portIndex) override {
        qDebug()<< "Performance In data recv";
    }

    virtual std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex const port) override {
        qDebug()<< "Performance Out data";
        auto data = std::make_shared<DefaultNodeData>();
        return data;
    }

    virtual QWidget *embeddedWidget() override{
        if (!m_InternalWidget) {
            m_InternalWidget = new PerformanceJobInternalWidget(this);

            PerformanceJobCollectionInfoList list;

            for (int i = 0; i < 6; ++i) {
                list.push_back({"path1/TestData" + std::to_string(i), i%2 == 1});
            }

            for (int i = 0; i < 6; ++i) {
                list.push_back({"path2/TestData" + std::to_string(i), i%2 == 1});
            }

            for (int i = 0; i < 6; ++i) {
                list.push_back({"path3/TestData" + std::to_string(i), i%2 == 1});
            }

            for (int i = 0; i < 6; ++i) {
                list.push_back({"TestData" + std::to_string(i), i%2 == 1});
            }

            m_InternalWidget->updateCollection(list);
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
    PerformanceJobInternalWidget* m_InternalWidget = nullptr;
};
