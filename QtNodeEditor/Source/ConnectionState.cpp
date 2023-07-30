#include "ConnectionState.hpp"

#include <QtCore/QDebug>
#include <QtCore/QPointF>

#include "BasicGraphicsScene.hpp"
#include "ConnectionGraphicsObject.hpp"
#include "NodeGraphicsObject.hpp"

namespace QtNodes {

    ConnectionState::~ConnectionState() {
        //resetLastHoveredNode();
    }

    PortType ConnectionState::requiredPort() const {
        PortType t = PortType::None;

        if (_cgo.connectionId().outNodeId == InvalidNodeId) {
            t = PortType::Out;
        } else if (_cgo.connectionId().inNodeId == InvalidNodeId) {
            t = PortType::In;
        }

        return t;
    }

    bool ConnectionState::requiresPort() const {
        const ConnectionId &id = _cgo.connectionId();
        return id.outNodeId == InvalidNodeId || id.inNodeId == InvalidNodeId;
    }

    bool ConnectionState::hovered() const {
        return m_Hovered;
    }

    void ConnectionState::setHovered(bool hovered) {
        m_Hovered = hovered;
    }

    void ConnectionState::setLastHoveredNode(NodeId const nodeId) {
        _lastHoveredNode = nodeId;
    }

    NodeId ConnectionState::lastHoveredNode() const {
        return _lastHoveredNode;
    }

    void ConnectionState::resetLastHoveredNode() {
        if (_lastHoveredNode != InvalidNodeId) {
            auto ngo = _cgo.nodeScene()->nodeGraphicsObject(_lastHoveredNode);
            ngo->update();
        }

        _lastHoveredNode = InvalidNodeId;
    }

    bool ConnectionState::isConnectionWidgetHovered() const {
        return m_ConnectionWidgetHovered;
    }

    void ConnectionState::setConnectionWidgetHovered(bool hovered) {
        m_ConnectionWidgetHovered = hovered;
    }

    bool ConnectionState::isConnectionWidgetSelected() const {
        return m_ConnectionWidgetSelected;
    }

    void ConnectionState::setConnectionWidgetSelected(bool selected) {
        m_ConnectionWidgetSelected = selected;
    }

    bool ConnectionState::isConnectionLineSelected() const {
        return m_ConnectionLineSelected;
    }

    void ConnectionState::setConnectionLineSelected(bool selected) {
        m_ConnectionLineSelected = selected;
    }

    bool ConnectionState::isConnectionWidgetPressed() const {
        return m_ConnectionWidgetPressed;
    }

    void ConnectionState::setConnectionWidgetPressed(bool pressed) {
        m_ConnectionWidgetPressed = pressed;
    }

} // namespace QtNodes
