#pragma once

#include <QCheckBox>
#include "QtNodes/NodeDelegateModel"
#include "DefaultNodeData.h"

class ConditionDelegateModel : public QtNodes::NodeDelegateModel  {
Q_OBJECT
public:
    ConditionDelegateModel(){ }

    virtual ~ConditionDelegateModel() { }

    virtual bool captionVisible() const override { return false; }

    virtual QString caption() const override { return "Condition";};

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

    virtual QString name() const override { return "ConditionDelegateModel"; };

//    QJsonObject save() const override;
//
//    void load(QJsonObject const &) override;

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
        qDebug()<< "Condition In data recv";
    }

    virtual std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex const port) override {
        qDebug()<< "Condition Out data";
        auto data = std::make_shared<DefaultNodeData>();
        return data;
    }

    virtual QWidget *embeddedWidget() override{
        if (!m_CheckBox) {
            m_CheckBox = new QCheckBox();
            m_CheckBox->setText("Some Conditions!");
        }

        return m_CheckBox;
    }

    virtual bool resizable() const { return true; }


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
    QCheckBox* m_CheckBox = nullptr;
};

