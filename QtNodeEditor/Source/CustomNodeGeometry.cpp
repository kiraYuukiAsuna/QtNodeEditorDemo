#include "CustomNodeGeometry.hpp"

#include "AbstractGraphModel.hpp"
#include "NodeData.hpp"

#include <QPoint>
#include <QRect>
#include <QWidget>

namespace QtNodes {

    CustomNodeGeometry::CustomNodeGeometry(AbstractGraphModel &graphModel)
            : AbstractNodeGeometry(graphModel), _portSize(20), _portSpasing(10), _fontMetrics(QFont()),
              _boldFontMetrics(QFont()),
              m_Width(180), m_Height(120),
              m_TitleBarHeight(0), m_TitleBarWidth(0),
              m_ContentAreaHeight(0), m_ContentAreaWidth(0) {
        QFont f;
        f.setBold(true);
        _boldFontMetrics = QFontMetrics(f);

        _portSize = _fontMetrics.height();
    }

    QSize CustomNodeGeometry::size(NodeId const nodeId) const {
        return _graphModel.nodeData<QSize>(nodeId, NodeRole::Size);
    }

    void CustomNodeGeometry::recomputeSize(NodeId const nodeId) const {
//        unsigned int height = maxVerticalPortsExtent(nodeId);
//
//        if (auto w = _graphModel.nodeData<QWidget *>(nodeId, NodeRole::Widget)) {
//            height = std::max(height, static_cast<unsigned int>(w->height()));
//        }
//
//        QRectF const capRect = captionRect(nodeId);
//
//        height += capRect.height();
//
//        height += _portSpasing; // space above caption
//        height += _portSpasing; // space below caption
//
//        unsigned int inPortWidth = maxPortsTextAdvance(nodeId, PortType::In);
//        unsigned int outPortWidth = maxPortsTextAdvance(nodeId, PortType::Out);
//
//        unsigned int width = inPortWidth + outPortWidth + 4 * _portSpasing;
//
//        if (auto w = _graphModel.nodeData<QWidget *>(nodeId, NodeRole::Widget)) {
//            width += w->width();
//        }
//
//        width = std::max(width, static_cast<unsigned int>(capRect.width()) + 2 * _portSpasing);
//
//        QSize size(width, height);
//
//        _graphModel.setNodeData(nodeId, NodeRole::Size, size);

        // ----------------------------- //

        // titlebar area
        QRectF const titleBarTextBoundingRect = captionRect(nodeId);
        m_TitleBarWidth = titleBarTextBoundingRect.width();
        m_TitleBarHeight = titleBarTextBoundingRect.height();

        auto embededWidget = _graphModel.nodeData<QWidget *>(nodeId, NodeRole::Widget);
        // content area
        if (embededWidget != nullptr) {
            embededWidget->adjustSize();
            m_ContentAreaWidth = embededWidget->width();
            m_ContentAreaHeight = embededWidget->height();
        } else {
            m_ContentAreaWidth = m_TitleBarWidth;
            m_ContentAreaHeight = 0;
        }

        // left icon
        QSize leftIconSize = {32,32};
        QSize rightIconSize = {32,32};

        // total
        m_Width = std::max(m_TitleBarWidth + 64, m_ContentAreaWidth + 4 * (int)_portSpasing + (int)maxPortsTextAdvance(nodeId, PortType::In) + (int)maxPortsTextAdvance(nodeId, PortType::Out));

        if (m_Width > m_ContentAreaWidth + 4 * (int)_portSpasing + (int)maxPortsTextAdvance(nodeId, PortType::In) +  (int)maxPortsTextAdvance(nodeId, PortType::Out)) {
            m_ContentAreaWidth = m_Width - 4 * (int)_portSpasing - (int)maxPortsTextAdvance(nodeId, PortType::In) -  (int)maxPortsTextAdvance(nodeId, PortType::Out);
            if (embededWidget != nullptr) {
                auto ratio = (float)embededWidget->width() / m_ContentAreaWidth;
                embededWidget->resize(m_ContentAreaWidth, embededWidget->height() * ratio);
            } else {
                m_ContentAreaHeight = 0;
            }

        } else {
            m_TitleBarWidth = m_Width - 64;
        }

        unsigned int step = _fontMetrics.height();
        int inputCount =  _graphModel.nodeData<unsigned int>(nodeId, NodeRole::InPortCount);
        int outputCount = _graphModel.nodeData<unsigned int>(nodeId, NodeRole::OutPortCount);
        if (m_ContentAreaHeight < step * std::max({outputCount, inputCount})) {
            m_ContentAreaHeight = step * std::max({outputCount, inputCount});
        }

        m_Height = std::max(m_TitleBarHeight, 32) + m_ContentAreaHeight + 2 * _portSpasing;

        QSize size(m_Width, m_Height);

        _graphModel.setNodeData(nodeId, NodeRole::Size, size);
    }

    QPointF CustomNodeGeometry::portPosition(NodeId const nodeId,
                                                        PortType const portType,
                                                        PortIndex const portIndex) const {
        unsigned int const step = _portSize + _portSpasing;

        QPointF result;

        double totalHeight = 0.0;

        totalHeight += std::max({32,m_TitleBarHeight});
        totalHeight += _portSpasing;

        totalHeight += step * portIndex;
        totalHeight += step / 2.0;

        QSize size = _graphModel.nodeData<QSize>(nodeId, NodeRole::Size);

        switch (portType) {
            case PortType::In: {
                double x = 0.0;

                result = QPointF(x, totalHeight);
                break;
            }

            case PortType::Out: {
                double x = size.width();

                result = QPointF(x, totalHeight);
                break;
            }

            default:
                break;
        }

        return result;
    }

    QPointF CustomNodeGeometry::portTextPosition(NodeId const nodeId,
                                                            PortType const portType,
                                                            PortIndex const portIndex) const {
        QPointF p = portPosition(nodeId, portType, portIndex);

        QRectF rect = portTextRect(nodeId, portType, portIndex);

        p.setY(p.y() + rect.height() / 4.0);

        QSize size = _graphModel.nodeData<QSize>(nodeId, NodeRole::Size);

        switch (portType) {
            case PortType::In:
                p.setX(_portSpasing);
                break;

            case PortType::Out:
                p.setX(size.width() - _portSpasing - rect.width());
                break;

            default:
                break;
        }

        return p;
    }

    QRectF CustomNodeGeometry::captionRect(NodeId const nodeId) const {
        if (!_graphModel.nodeData<bool>(nodeId, NodeRole::CaptionVisible))
            return QRect();

        QString name = _graphModel.nodeData<QString>(nodeId, NodeRole::Caption);

        return _boldFontMetrics.boundingRect(name);
    }

    QPointF CustomNodeGeometry::captionPosition(NodeId const nodeId) const {
        QSize size = _graphModel.nodeData<QSize>(nodeId, NodeRole::Size);
//        return QPointF(0.5 * (size.width() - captionRect(nodeId).width() - 64) + 32,
//                       (float)2/3 * std::max({(int)captionRect(nodeId).height(), 32}));
        return QPointF(32 + 3,
                       (float)2/3 * std::max({(int)captionRect(nodeId).height(), 32}));
    }

    QPointF CustomNodeGeometry::widgetPosition(NodeId const nodeId) const {
        QSize size = _graphModel.nodeData<QSize>(nodeId, NodeRole::Size);

        unsigned int captionHeight = captionRect(nodeId).height();

        if (auto w = _graphModel.nodeData<QWidget *>(nodeId, NodeRole::Widget)) {
//            // If the widget wants to use as much vertical space as possible,
//            // place it immediately after the caption.
//            if (w->sizePolicy().verticalPolicy() & QSizePolicy::ExpandFlag) {
//                return QPointF(2.0 * _portSpasing + maxPortsTextAdvance(nodeId, PortType::In),
//                               captionHeight);
//            } else {
//                return QPointF(2.0 * _portSpasing + maxPortsTextAdvance(nodeId, PortType::In),
//                               (captionHeight + size.height() - w->height()) / 2.0);
//            }
            int yPos = std::max({32, m_TitleBarHeight}) + _portSpasing + (m_ContentAreaHeight - w->height()) / 2;
            return QPointF(2 * _portSpasing + maxPortsTextAdvance(nodeId, PortType::In), yPos);
        }
        return QPointF();
    }

    QRect CustomNodeGeometry::resizeHandleRect(NodeId const nodeId) const {
        QSize size = _graphModel.nodeData<QSize>(nodeId, NodeRole::Size);

        unsigned int rectSize = 7;

        return QRect(size.width() - _portSpasing, size.height() - _portSpasing, rectSize, rectSize);
    }

    QRectF CustomNodeGeometry::portTextRect(NodeId const nodeId,
                                                       PortType const portType,
                                                       PortIndex const portIndex) const {
        QString s;
        if (_graphModel.portData<bool>(nodeId, portType, portIndex, PortRole::CaptionVisible)) {
            s = _graphModel.portData<QString>(nodeId, portType, portIndex, PortRole::Caption);
        } else {
            auto portData = _graphModel.portData(nodeId, portType, portIndex, PortRole::DataType);

            s = portData.value<NodeDataType>().name;
        }

        return _fontMetrics.boundingRect(s);
    }

    unsigned int CustomNodeGeometry::maxVerticalPortsExtent(NodeId const nodeId) const {
        PortCount nInPorts = _graphModel.nodeData<PortCount>(nodeId, NodeRole::InPortCount);

        PortCount nOutPorts = _graphModel.nodeData<PortCount>(nodeId, NodeRole::OutPortCount);

        unsigned int maxNumOfEntries = std::max(nInPorts, nOutPorts);
        unsigned int step = _portSize + _portSpasing;

        return step * maxNumOfEntries;
    }

    unsigned int CustomNodeGeometry::maxPortsTextAdvance(NodeId const nodeId,
                                                                    PortType const portType) const {
        unsigned int width = 0;

        size_t const n = _graphModel
                .nodeData(nodeId,
                          (portType == PortType::Out) ? NodeRole::OutPortCount
                                                      : NodeRole::InPortCount)
                .toUInt();

        for (PortIndex portIndex = 0ul; portIndex < n; ++portIndex) {
            QString name;

            if (_graphModel.portData<bool>(nodeId, portType, portIndex, PortRole::CaptionVisible)) {
                name = _graphModel.portData<QString>(nodeId, portType, portIndex, PortRole::Caption);
            } else {
                NodeDataType portData = _graphModel.portData<NodeDataType>(nodeId,
                                                                           portType,
                                                                           portIndex,
                                                                           PortRole::DataType);

                name = portData.name;
            }
            width = std::max(unsigned(_fontMetrics.horizontalAdvance(name)), width);
        }

        return width;
    }

    QPointF CustomNodeGeometry::deleteIconPosition(const NodeId nodeId) const {
        return {static_cast<qreal>(m_Width - 32), 0};
    }

    QRectF CustomNodeGeometry::deleteIconRect(const NodeId nodeId) const {
        return {0, 0, 32, 32};
    }

} // namespace QtNodes
