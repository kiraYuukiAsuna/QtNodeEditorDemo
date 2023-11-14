#include "ConnectionGraphicsObject.hpp"

#include "AbstractGraphModel.hpp"
#include "AbstractNodeGeometry.hpp"
#include "BasicGraphicsScene.hpp"
#include "ConnectionIdUtils.hpp"
#include "ConnectionPainter.hpp"
#include "ConnectionState.hpp"
#include "ConnectionStyle.hpp"
#include "NodeConnectionInteraction.hpp"
#include "NodeGraphicsObject.hpp"
#include "StyleCollection.hpp"
#include "locateNode.hpp"
#include "ConnectionPopupWindow.h"

#include <QtWidgets/QGraphicsBlurEffect>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtCore/QDebug>

#include <stdexcept>
#include <QMessageBox>
#include <utility>

namespace QtNodes {

    ConnectionGraphicsObject::ConnectionGraphicsObject(BasicGraphicsScene &scene,
                                                       ConnectionId const connectionId, QJsonObject conditionDataJsonObject)
            : _connectionId(connectionId), _graphModel(scene.graphModel()), _connectionState(*this), _out{0, 0},
              _in{0, 0}, m_ConnectionGeometry(this) {
        scene.addItem(this);


        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemIsFocusable, true);
        setFlag(QGraphicsItem::ItemIsSelectable, true);

        setAcceptHoverEvents(true);

        //addGraphicsEffect();

        setZValue(-1.0);

        initializePosition();

        m_ConditionDataJsonObject = std::move(conditionDataJsonObject);

        m_ConnectionGeometry.updateGeometry();
    }

    void ConnectionGraphicsObject::initializePosition() {
        // This function is only called when the ConnectionGraphicsObject
        // is newly created. At this moment both end coordinates are (0, 0)
        // in Connection G.O. coordinates. The position of the whole
        // Connection G. O. in scene coordinate system is also (0, 0).
        // By moving the whole object to the Node Port position
        // we position both connection ends correctly.

        if (_connectionState.requiredPort() != PortType::None) {
            PortType attachedPort = oppositePort(_connectionState.requiredPort());

            PortIndex portIndex = getPortIndex(attachedPort, _connectionId);
            NodeId nodeId = getNodeId(attachedPort, _connectionId);

            NodeGraphicsObject *ngo = nodeScene()->nodeGraphicsObject(nodeId);

            if (ngo) {
                QTransform nodeSceneTransform = ngo->sceneTransform();

                AbstractNodeGeometry &geometry = nodeScene()->nodeGeometry();

                QPointF pos = geometry.portScenePosition(nodeId,
                                                         attachedPort,
                                                         portIndex,
                                                         nodeSceneTransform);

                this->setPos(pos);
            }
        }

        move();
    }

    AbstractGraphModel &ConnectionGraphicsObject::graphModel() const {
        return _graphModel;
    }

    BasicGraphicsScene *ConnectionGraphicsObject::nodeScene() const {
        return dynamic_cast<BasicGraphicsScene *>(scene());
    }

    ConnectionId const &ConnectionGraphicsObject::connectionId() const {
        return _connectionId;
    }

    QRectF ConnectionGraphicsObject::boundingRect() const {
        auto points = pointsC1C2();

        // `normalized()` fixes inverted rects.
        QRectF basicRect = QRectF(_out, _in).normalized();

        QRectF c1c2Rect = QRectF(points.first, points.second).normalized();

        QRectF commonRect = basicRect.united(c1c2Rect);

        auto const &connectionStyle = StyleCollection::connectionStyle();
        float const diam = connectionStyle.pointDiameter();
        QPointF const cornerOffset(diam, diam);

        // Expand rect by port circle diameter
        commonRect.setTopLeft(commonRect.topLeft() - cornerOffset);
        commonRect.setBottomRight(commonRect.bottomRight() + 2 * cornerOffset);

        return commonRect;
    }

    QPainterPath ConnectionGraphicsObject::shape() const {
#ifdef DEBUG_DRAWING

        //QPainterPath path;

        //path.addRect(boundingRect());
        //return path;

#else
        return ConnectionPainter::getPainterStroke(const_cast<ConnectionGraphicsObject*>(this));
#endif
    }

    QPointF const &ConnectionGraphicsObject::endPoint(PortType portType) const {
        Q_ASSERT(portType != PortType::None);

        return (portType == PortType::Out ? _out : _in);
    }

    void ConnectionGraphicsObject::setEndPoint(PortType portType, QPointF const &point) {
        if (portType == PortType::In)
            _in = point;
        else
            _out = point;
    }

    void ConnectionGraphicsObject::move() {
        auto moveEnd = [this](ConnectionId cId, PortType portType) {
            NodeId nodeId = getNodeId(portType, cId);

            if (nodeId == InvalidNodeId)
                return;

            NodeGraphicsObject *ngo = nodeScene()->nodeGraphicsObject(nodeId);

            if (ngo) {
                AbstractNodeGeometry &geometry = nodeScene()->nodeGeometry();

                QPointF scenePos = geometry.portScenePosition(nodeId,
                                                              portType,
                                                              getPortIndex(portType, cId),
                                                              ngo->sceneTransform());

                QPointF connectionPos = sceneTransform().inverted().map(scenePos);

                setEndPoint(portType, connectionPos);
            }
        };

        moveEnd(_connectionId, PortType::Out);
        moveEnd(_connectionId, PortType::In);

        prepareGeometryChange();

        update();
    }

    ConnectionState const &ConnectionGraphicsObject::connectionState() const {
        return _connectionState;
    }

    ConnectionState &ConnectionGraphicsObject::connectionState() {
        return _connectionState;
    }

    void ConnectionGraphicsObject::paint(QPainter *painter,
                                         QStyleOptionGraphicsItem const *option,
                                         QWidget *) {
        m_ConnectionGeometry.updateGeometry();

        if (!scene())
            return;

        painter->setClipRect(option->exposedRect);

        ConnectionPainter::paint(painter, this);
    }

    void ConnectionGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event) {
        // connection widget
        QPointF localPos = event->pos();
        if(m_ConnectionGeometry.getConditionWidgetRect().contains(localPos)){
            _connectionState.setConnectionLineSelected(false);
            _connectionState.setConnectionWidgetPressed(true);
            _connectionState.setConnectionWidgetPressed(true);
            qDebug() << "Press Mouse:" << localPos;
        }else{
            _connectionState.setConnectionLineSelected(true);
            _connectionState.setConnectionWidgetPressed(false);
        }

        QGraphicsItem::mousePressEvent(event);

        update();
    }

    void ConnectionGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
        prepareGeometryChange();

        auto view = static_cast<QGraphicsView *>(event->widget());
        auto ngo = locateNodeAt(event->scenePos(), *nodeScene(), view->transform());
        if (ngo) {
            ngo->reactToConnection(this);

            _connectionState.setLastHoveredNode(ngo->nodeId());
        } else {
            _connectionState.resetLastHoveredNode();
        }

        //-------------------

        auto requiredPort = _connectionState.requiredPort();

        if (requiredPort != PortType::None) {
            setEndPoint(requiredPort, event->pos());
        }

        //-------------------

        update();

        event->accept();
    }

    void ConnectionGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
        QGraphicsItem::mouseReleaseEvent(event);

        QPointF localPos = event->pos();

        if(m_ConnectionGeometry.getConditionWidgetRect().contains(localPos) && _connectionState.isConnectionWidgetPressed()){
            auto* graphicsView = static_cast<QGraphicsView *>(event->widget());

            Q_ASSERT(graphicsView);

            ConnectionPopupWindow view{m_ConditionDataJsonObject, nullptr};

            if (view.exec() == QDialog::Accepted) {
                m_ConditionDataJsonObject = view.save();
            }
        }

        _connectionState.setConnectionWidgetPressed(false);
        qDebug() << "Release Mouse:" << localPos;

        update();

        ungrabMouse();
        event->accept();

        auto view = static_cast<QGraphicsView *>(event->widget());

        Q_ASSERT(view);

        auto ngo = locateNodeAt(event->scenePos(), *nodeScene(), view->transform());

        bool wasConnected = false;

        if (ngo) {
            NodeConnectionInteraction interaction(*ngo, *this, *nodeScene());

            wasConnected = interaction.tryConnect();
        }

        // If connection attempt was unsuccessful
        if (!wasConnected) {
            // Resulting unique_ptr is not used and automatically deleted.
            nodeScene()->resetDraftConnection();
        }

    }

    void ConnectionGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
        _connectionState.setHovered(true);

        // Signal
        nodeScene()->connectionHovered(connectionId(), event->screenPos());

        // connection widget
        QPointF localPos = event->pos();
        if(m_ConnectionGeometry.getConditionWidgetRect().contains(localPos)){
            _connectionState.setConnectionWidgetHovered(true);
            qDebug() << "Inside Local position:" << localPos;
        }

        update();

        event->accept();
    }

    void ConnectionGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
        _connectionState.setHovered(false);

        // Signal
        nodeScene()->connectionHoverLeft(connectionId());

        // connection widget
        QPointF localPos = event->pos();
        qDebug() << "Outside - Local position: " << localPos;
        _connectionState.setConnectionWidgetHovered(false);

        update();

        event->accept();
    }

    std::pair<QPointF, QPointF> ConnectionGraphicsObject::pointsC1C2() const {
        switch (nodeScene()->orientation()) {
            case Qt::Horizontal:
                return pointsC1C2Horizontal();
                break;

            case Qt::Vertical:
                return pointsC1C2Vertical();
                break;
        }

        throw std::logic_error("Unreachable code after switch statement");
    }

    void ConnectionGraphicsObject::addGraphicsEffect() {
        auto effect = new QGraphicsBlurEffect;

        effect->setBlurRadius(5);
        setGraphicsEffect(effect);

        //auto effect = new QGraphicsDropShadowEffect;
        //auto effect = new ConnectionBlurEffect(this);
        //effect->setOffset(4, 4);
        //effect->setColor(QColor(Qt::gray).darker(800));
    }

    std::pair<QPointF, QPointF> ConnectionGraphicsObject::pointsC1C2Horizontal() const {
        double const defaultOffset = 200;

        double xDistance = _in.x() - _out.x();

        double horizontalOffset = qMin(defaultOffset, std::abs(xDistance));

        double verticalOffset = 0;

        double ratioX = 0.5;

        if (xDistance <= 0) {
            double yDistance = _in.y() - _out.y() + 20;

            double vector = yDistance < 0 ? -1.0 : 1.0;

            verticalOffset = qMin(defaultOffset, std::abs(yDistance)) * vector;

            ratioX = 1.0;
        }

        horizontalOffset *= ratioX;

        QPointF c1(_out.x() + horizontalOffset, _out.y() + verticalOffset);

        QPointF c2(_in.x() - horizontalOffset, _in.y() - verticalOffset);

        return std::make_pair(c1, c2);
    }

    std::pair<QPointF, QPointF> ConnectionGraphicsObject::pointsC1C2Vertical() const {
        double const defaultOffset = 200;

        double yDistance = _in.y() - _out.y();

        double verticalOffset = qMin(defaultOffset, std::abs(yDistance));

        double horizontalOffset = 0;

        double ratioY = 0.5;

        if (yDistance <= 0) {
            double xDistance = _in.x() - _out.x() + 20;

            double vector = xDistance < 0 ? -1.0 : 1.0;

            horizontalOffset = qMin(defaultOffset, std::abs(xDistance)) * vector;

            ratioY = 1.0;
        }

        verticalOffset *= ratioY;

        QPointF c1(_out.x() + horizontalOffset, _out.y() + verticalOffset);

        QPointF c2(_in.x() - horizontalOffset, _in.y() - verticalOffset);

        return std::make_pair(c1, c2);
    }

} // namespace QtNodes
