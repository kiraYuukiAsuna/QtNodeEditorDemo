#pragma once

#include "AbstractNodeGeometry.hpp"

#include <QtGui/QFontMetrics>

namespace QtNodes {

    class AbstractGraphModel;

    class BasicGraphicsScene;

    class NODE_EDITOR_PUBLIC CustomNodeGeometry : public AbstractNodeGeometry {
    public:
        CustomNodeGeometry(AbstractGraphModel &graphModel);

    public:
        QSize size(NodeId const nodeId) const override;

        void recomputeSize(NodeId const nodeId) const override;

        QPointF portPosition(NodeId const nodeId,
                             PortType const portType,
                             PortIndex const index) const override;

        QPointF portTextPosition(NodeId const nodeId,
                                 PortType const portType,
                                 PortIndex const PortIndex) const override;

        QPointF captionPosition(NodeId const nodeId) const override;

        QRectF captionRect(NodeId const nodeId) const override;

        QPointF widgetPosition(NodeId const nodeId) const override;

        QRect resizeHandleRect(NodeId const nodeId) const override;

        QPointF deleteIconPosition(NodeId const nodeId) const override;

        QRectF deleteIconRect(NodeId const nodeId) const override;

    private:
        QRectF portTextRect(NodeId const nodeId,
                            PortType const portType,
                            PortIndex const portIndex) const;

        /// Finds max number of ports and multiplies by (a port height + interval)
        unsigned int maxVerticalPortsExtent(NodeId const nodeId) const;

        unsigned int maxPortsTextAdvance(NodeId const nodeId, PortType const portType) const;

    private:
        // Some variables are mutable because we need to change drawing
        // metrics corresponding to fontMetrics but this doesn't change
        // constness of the Node.

        mutable unsigned int _portSize;
        unsigned int _portSpasing;
        mutable QFontMetrics _fontMetrics;
        mutable QFontMetrics _boldFontMetrics;

    public:
        mutable int m_Width;
        mutable int m_Height;
        mutable int m_TitleBarHeight;
        mutable int m_TitleBarWidth;
        mutable int m_ContentAreaHeight;
        mutable int m_ContentAreaWidth;
        const int32_t _titleBarExpand = 48;
        const int32_t _iconSize = 64;
        const float _nodeEdgeRadius = 12.0;

    };

} // namespace QtNodes
