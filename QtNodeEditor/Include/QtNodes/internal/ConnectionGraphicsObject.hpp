#pragma once

#include <utility>

#include <QtCore/QUuid>
#include <QJsonObject>
#include <QtWidgets/QGraphicsObject>
#include "ConnectionGeometry.hpp"
#include "ConnectionState.hpp"
#include "Definitions.hpp"

class QGraphicsSceneMouseEvent;

namespace QtNodes {

    class AbstractGraphModel;

    class BasicGraphicsScene;

/// Graphic Object for connection. Adds itself to scene
class ConnectionGraphicsObject : public QGraphicsObject {
    Q_OBJECT
    public:
        // Needed for qgraphicsitem_cast
        enum {
            Type = UserType + 2
        };

        int type() const override { return Type; }

    public:
        ConnectionGraphicsObject(BasicGraphicsScene &scene, ConnectionId const connectionId, QJsonObject conditionDataJsonObject);

        ~ConnectionGraphicsObject() = default;

    public:
        AbstractGraphModel &graphModel() const;

        BasicGraphicsScene *nodeScene() const;

        ConnectionId const &connectionId() const;

        QRectF boundingRect() const override;

        QPainterPath shape() const override;

        QPointF const &endPoint(PortType portType) const;

        QPointF out() const { return _out; }

        QPointF in() const { return _in; }

        std::pair<QPointF, QPointF> pointsC1C2() const;

        void setEndPoint(PortType portType, QPointF const &point);

        /// Updates the position of both ends
        void move();

        ConnectionState const &connectionState() const;

        ConnectionState &connectionState();

        ConnectionGeometry& getConnectionGeometry(){
            return m_ConnectionGeometry;
        }

        QJsonObject getConditionDataJsonObject(){
            return m_ConditionDataJsonObject;
        }

        void setConditionDataJsonObject(QJsonObject conditionDataJsonObject){
            m_ConditionDataJsonObject = conditionDataJsonObject;
        }

    protected:
        void paint(QPainter *painter,
                   QStyleOptionGraphicsItem const *option,
                   QWidget *widget = 0) override;

        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

        void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    private:
        void initializePosition();

        void addGraphicsEffect();

        std::pair<QPointF, QPointF> pointsC1C2Horizontal() const;

        std::pair<QPointF, QPointF> pointsC1C2Vertical() const;

    private:
        ConnectionId _connectionId;

        AbstractGraphModel &_graphModel;

        ConnectionState _connectionState;

        QJsonObject m_ConditionDataJsonObject;

        ConnectionGeometry m_ConnectionGeometry;

        mutable QPointF _out;
        mutable QPointF _in;
    };

} // namespace QtNodes
