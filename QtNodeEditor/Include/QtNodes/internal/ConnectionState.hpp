#pragma once

#include <QtCore/QUuid>

#include "Export.hpp"

#include "Definitions.hpp"

class QPointF;

namespace QtNodes {

    class ConnectionGraphicsObject;

/// Stores currently draggind end.
/// Remembers last hovered Node.
    class NODE_EDITOR_PUBLIC ConnectionState {
    public:
        /// Defines whether we construct a new connection
        /// or it is already binding two nodes.
        enum LooseEnd {
            Pending = 0, Connected = 1
        };

    public:
        ConnectionState(ConnectionGraphicsObject &cgo)
                : _cgo(cgo), m_Hovered(false), m_ConnectionWidgetHovered(false), m_ConnectionWidgetSelected(false), m_ConnectionLineSelected(
                false), m_ConnectionWidgetPressed(false) {}

        ConnectionState(ConnectionState const &) = delete;

        ConnectionState(ConnectionState &&) = delete;

        ConnectionState &operator=(ConnectionState const &) = delete;

        ConnectionState &operator=(ConnectionState &&) = delete;

        ~ConnectionState();

    public:
        PortType requiredPort() const;

        bool requiresPort() const;

        bool hovered() const;

        void setHovered(bool hovered);

        bool isConnectionWidgetHovered() const;

        void setConnectionWidgetHovered(bool hovered);

        bool isConnectionWidgetSelected() const;

        void setConnectionWidgetSelected(bool selected);

        bool isConnectionLineSelected() const;

        void setConnectionLineSelected(bool selected);

        bool isConnectionWidgetPressed() const;

        void setConnectionWidgetPressed(bool pressed);

    public:
        /// Caches NodeId for further interaction.
        void setLastHoveredNode(NodeId const nodeId);

        NodeId lastHoveredNode() const;

        void resetLastHoveredNode();

    private:
        ConnectionGraphicsObject &_cgo;

        bool m_Hovered;

        bool m_ConnectionWidgetHovered;

        bool m_ConnectionWidgetSelected;

        bool m_ConnectionLineSelected;

        bool m_ConnectionWidgetPressed;
//        bool m_ConnectionWidgetReleased;

        NodeId _lastHoveredNode{InvalidNodeId};
    };
} // namespace QtNodes
