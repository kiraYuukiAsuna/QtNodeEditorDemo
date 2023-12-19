#include "ConnectionPainter.hpp"

#include <QtGui/QIcon>

#include "AbstractGraphModel.hpp"
#include "ConnectionGraphicsObject.hpp"
#include "ConnectionState.hpp"
#include "Definitions.hpp"
#include "NodeData.hpp"
#include "StyleCollection.hpp"
#include "ConnectionGeometry.hpp"

namespace QtNodes {
    QPainterPath ConnectionPainter::getPainterStroke(ConnectionGraphicsObject *cgo) {
        auto path = orthogonalPath(cgo);

        QPointF const &out = cgo->endPoint(PortType::Out);
        QPainterPath result(out);

        unsigned segments = 20;

        for (auto i = 0ul; i < segments; ++i) {
            double ratio = double(i + 1) / segments;
            result.lineTo(path.pointAtPercent(ratio));
        }

        QPainterPathStroker stroker;
        stroker.setWidth(10.0);

        return stroker.createStroke(result);
    }

#ifdef NODE_DEBUG_DRAWING
    static void debugDrawing(QPainter *painter, ConnectionGraphicsObject* const &cgo)
    {
        Q_UNUSED(painter);

        {
            QPointF const &in = cgo->endPoint(PortType::In);
            QPointF const &out = cgo->endPoint(PortType::Out);

            auto const points = cgo->pointsC1C2();

            painter->setPen(Qt::red);
            painter->setBrush(Qt::red);

            painter->drawLine(QLineF(out, points.first));
            painter->drawLine(QLineF(points.first, points.second));
            painter->drawLine(QLineF(points.second, in));
            painter->drawEllipse(points.first, 3, 3);
            painter->drawEllipse(points.second, 3, 3);

            painter->setBrush(Qt::NoBrush);
            painter->drawPath(orthogonalPath(cgo));
        }

        {
            painter->setPen(Qt::yellow);
            painter->drawRect(cgo->boundingRect());
        }
    }

#endif

    static void drawSketchLine(QPainter *painter, ConnectionGraphicsObject *cgo) {
        ConnectionState const &state = cgo->connectionState();

        if (state.requiresPort()) {
            auto const &connectionStyle = QtNodes::StyleCollection::connectionStyle();

            QPen pen;
            pen.setWidth(connectionStyle.constructionLineWidth());
            pen.setColor(connectionStyle.constructionColor());
            pen.setStyle(Qt::DashLine);

            painter->setPen(pen);
            painter->setBrush(Qt::NoBrush);

            auto path = orthogonalPath(cgo);

            painter->drawPath(path);
        }
    }

    static void drawHoveredOrSelected(QPainter *painter, ConnectionGraphicsObject *cgo) {
        bool const hovered = cgo->connectionState().hovered();
        bool const selected = cgo->isSelected();

        // drawn as a fat background
        if (hovered && !cgo->connectionState().isConnectionWidgetHovered()) {
            auto const &connectionStyle = QtNodes::StyleCollection::connectionStyle();

            double const lineWidth = connectionStyle.lineWidth();

            QPen pen;
            pen.setWidth(2 * lineWidth);
            pen.setColor(connectionStyle.hoveredColor());

            painter->setPen(pen);
            painter->setBrush(Qt::NoBrush);

            auto const path = orthogonalPath(cgo);
            painter->drawPath(path);
        }

        if (selected && cgo->connectionState().isConnectionLineSelected()) {
            auto const &connectionStyle = QtNodes::StyleCollection::connectionStyle();

            double const lineWidth = connectionStyle.lineWidth();

            QPen pen;
            pen.setWidth(2 * lineWidth);
            pen.setColor(connectionStyle.selectedHaloColor());

            painter->setPen(pen);
            painter->setBrush(Qt::NoBrush);

            auto const path = orthogonalPath(cgo);
            painter->drawPath(path);
        }
    }

    static void drawNormalLine(QPainter *painter, ConnectionGraphicsObject *cgo) {
        ConnectionState const &state = cgo->connectionState();

        if (state.requiresPort())
            return;

        // colors
        auto const &connectionStyle = QtNodes::StyleCollection::connectionStyle();

        QColor normalColorOut = connectionStyle.normalColor();
        QColor normalColorIn = connectionStyle.normalColor();
        QColor selectedColor = connectionStyle.selectedColor();

        bool useGradientColor = false;

        AbstractGraphModel const &graphModel = cgo->graphModel();

        if (connectionStyle.useDataDefinedColors()) {
            using QtNodes::PortType;

            auto const cId = cgo->connectionId();

            auto dataTypeOut = graphModel
                    .portData(cId.outNodeId,
                              PortType::Out,
                              cId.outPortIndex,
                              PortRole::DataType)
                    .value<NodeDataType>();

            auto dataTypeIn
                    = graphModel.portData(cId.inNodeId, PortType::In, cId.inPortIndex, PortRole::DataType)
                    .value<NodeDataType>();

            useGradientColor = (dataTypeOut.id != dataTypeIn.id);

            normalColorOut = connectionStyle.normalColor(dataTypeOut.id);
            normalColorIn = connectionStyle.normalColor(dataTypeIn.id);
            selectedColor = normalColorOut.darker(200);
        }

        // geometry

        double const lineWidth = connectionStyle.lineWidth();

        // draw normal line
        QPen p;

        p.setWidth(lineWidth);

        bool const selected = cgo->isSelected();

        useGradientColor = false;
        auto path = orthogonalPath(cgo);
        if (useGradientColor) {
            painter->setBrush(Qt::NoBrush);

            QColor cOut = normalColorOut;
            if (selected && cgo->connectionState().isConnectionLineSelected()) {
                cOut = cOut.darker(200);
            }
            p.setColor(cOut);
            painter->setPen(p);

            unsigned int const segments = 60;

            for (unsigned int i = 0ul; i < segments; ++i) {
                double ratioPrev = double(i) / segments;
                double ratio = double(i + 1) / segments;

                if (i == segments / 2) {
                    QColor cIn = normalColorIn;
                    if (selected && cgo->connectionState().isConnectionLineSelected()) {
                        cIn = cIn.darker(200);
                    }

                    p.setColor(cIn);
                    painter->setPen(p);
                }
                painter->drawLine(path.pointAtPercent(ratioPrev), path.pointAtPercent(ratio));
            } {
                QString iconPath = ":/image/conditions.png";
                QColor backgroundColor = Qt::white;
                float shrinkRatio = 1.0;
                if (cgo->connectionState().isConnectionWidgetPressed()) {
                    iconPath = ":/image/setting-fill.png";
                    backgroundColor = QColor{0xF0F0F0};
                    shrinkRatio = 0.8;
                } else if (cgo->connectionState().isConnectionWidgetHovered()) {
                    iconPath = ":/image/setting-fill.png";
                    backgroundColor = QColor{0xF0F0F0};
                    shrinkRatio = 1.0;
                } else {
                    iconPath = ":/image/setting-filling.png";
                    backgroundColor = QColor{0xF0F0F0};
                    shrinkRatio = 0.8;
                }

                QPixmap originalPixmap(iconPath);
                QPixmap pixmap = originalPixmap.scaled(QSize(
                    cgo->getConnectionGeometry().getShrinkIconSize(shrinkRatio).width(),
                    cgo->getConnectionGeometry().getShrinkIconSize(shrinkRatio).height()));

                p.setColor(backgroundColor);
                painter->setBrush(backgroundColor);
                painter->setPen(p);
                painter->drawEllipse(cgo->getConnectionGeometry().getShrinkIconPosition(shrinkRatio) + QPointF{
                                         cgo->getConnectionGeometry().getShrinkConditionWidgetRect(shrinkRatio).width()
                                         / 2,
                                         cgo->getConnectionGeometry().getShrinkConditionWidgetRect(shrinkRatio).height()
                                         / 2
                                     }
                                     , cgo->getConnectionGeometry().getShrinkConditionWidgetRect(shrinkRatio).width() /
                                       2, cgo->getConnectionGeometry().getShrinkConditionWidgetRect(shrinkRatio).
                                          height() / 2);
                painter->drawPixmap(cgo->getConnectionGeometry().getShrinkIconPosition(shrinkRatio), pixmap);
            }
        } else {
            p.setColor(normalColorOut);

            if (selected && cgo->connectionState().isConnectionLineSelected()) {
                p.setColor(selectedColor);
            }

            painter->setPen(p);
            painter->setBrush(Qt::NoBrush);

            painter->drawPath(path);
        }
    }

    void ConnectionPainter::paint(QPainter *painter, ConnectionGraphicsObject *cgo) {
        drawHoveredOrSelected(painter, cgo);

        drawSketchLine(painter, cgo);

        drawNormalLine(painter, cgo);

#ifdef NODE_DEBUG_DRAWING
        debugDrawing(painter, cgo);
#endif

        // draw end points
        auto const &connectionStyle = QtNodes::StyleCollection::connectionStyle();

        double const pointDiameter = connectionStyle.pointDiameter();

        painter->setPen(connectionStyle.constructionColor());
        painter->setBrush(connectionStyle.constructionColor());
        double const pointRadius = pointDiameter / 2.0;
        painter->drawEllipse(cgo->out(), pointRadius, pointRadius);
        painter->drawEllipse(cgo->in(), pointRadius, pointRadius);
    }
} // namespace QtNodes
