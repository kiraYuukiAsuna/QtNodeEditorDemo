#include "QtNodes/internal/CustomNodePainter.h"

#include <cmath>

#include <QtCore/QMargins>

#include "AbstractGraphModel.hpp"
#include "AbstractNodeGeometry.hpp"
#include "BasicGraphicsScene.hpp"
#include "ConnectionGraphicsObject.hpp"
#include "ConnectionIdUtils.hpp"
#include "NodeGraphicsObject.hpp"
#include "NodeState.hpp"
#include "StyleCollection.hpp"
#include "CustomNodeGeometry.hpp"

namespace QtNodes {

    void CustomNodePainter::paint(QPainter *painter, NodeGraphicsObject &ngo) const {
        // TODO?
        //AbstractNodeGeometry & geometry = ngo.nodeScene()->nodeGeometry();
        //geometry.recomputeSizeIfFontChanged(painter->font());

        drawNodeRect(painter, ngo);

        drawConnectionPoints(painter, ngo);

        drawFilledConnectionPoints(painter, ngo);

        drawNodeLeftIcon(painter, ngo);

        drawNodeRightIcon(painter, ngo);

        drawNodeCaption(painter, ngo);

        drawEntryLabels(painter, ngo);

        drawResizeRect(painter, ngo);
    }

    void CustomNodePainter::drawNodeRect(QPainter *painter, NodeGraphicsObject &ngo) const {
//        AbstractGraphModel &model = ngo.graphModel();
//
//        NodeId const nodeId = ngo.nodeId();
//
//        AbstractNodeGeometry &geometry = ngo.nodeScene()->nodeGeometry();
//
//        QSize size = geometry.size(nodeId);
//
//        QJsonDocument json = QJsonDocument::fromVariant(model.nodeData(nodeId, NodeRole::Style));
//
//        NodeStyle nodeStyle(json.object());
//
//        auto color = ngo.isSelected() ? nodeStyle.SelectedBoundaryColor : nodeStyle.NormalBoundaryColor;
//
//        if (ngo.nodeState().hovered()) {
//            QPen p(color, nodeStyle.HoveredPenWidth);
//            painter->setPen(p);
//        } else {
//            QPen p(color, nodeStyle.PenWidth);
//            painter->setPen(p);
//        }
//
//        QLinearGradient gradient(QPointF(0.0, 0.0), QPointF(2.0, size.height()));
//
//        gradient.setColorAt(0.0, nodeStyle.GradientColor0);
//        gradient.setColorAt(0.10, nodeStyle.GradientColor1);
//        gradient.setColorAt(0.90, nodeStyle.GradientColor2);
//        gradient.setColorAt(1.0, nodeStyle.GradientColor3);
//
//        painter->setBrush(gradient);
//
//        QRectF boundary(0, 0, size.width(), size.height());
//
//        double const radius = 3.0;
//
//        painter->drawRoundedRect(boundary, radius, radius);
        // ------------------ //
        AbstractGraphModel &model = ngo.graphModel();

        NodeId const nodeId = ngo.nodeId();

        QJsonDocument json = QJsonDocument::fromVariant(model.nodeData(nodeId, NodeRole::Style));

        NodeStyle nodeStyle(json.object());

        auto color = ngo.isSelected()
                     ? nodeStyle.SelectedBoundaryColor
                     : nodeStyle.NormalBoundaryColor;

        if (ngo.nodeState().hovered()) {
            QPen p(nodeStyle.SelectedBoundaryColor, nodeStyle.HoveredPenWidth);
            painter->setPen(p);
        } else {
            QPen p(nodeStyle.NormalBoundaryColor, nodeStyle.PenWidth);
            painter->setPen(p);
        }

        AbstractNodeGeometry &geometry = ngo.nodeScene()->nodeGeometry();

        auto geom = dynamic_cast<CustomNodeGeometry&>(geometry);

        QLinearGradient gradient(QPointF(0.0, 0.0),
                                 QPointF(2.0, geom.m_Height));

        gradient.setColorAt(0.0, nodeStyle.GradientColor0);
        gradient.setColorAt(0.03, nodeStyle.GradientColor1);
        gradient.setColorAt(0.97, nodeStyle.GradientColor2);
        gradient.setColorAt(1.0, nodeStyle.GradientColor3);

        painter->setBrush(gradient);

        QRectF boundary(0, 0, geom.m_Width, geom.m_Height);

        double const xRadius = 5.0;
        double const yRadius = 5.0;

        painter->drawRoundedRect(boundary, xRadius, yRadius);
    }

    void CustomNodePainter::drawConnectionPoints(QPainter *painter, NodeGraphicsObject &ngo) const {
        AbstractGraphModel &model = ngo.graphModel();
        NodeId const nodeId = ngo.nodeId();
        AbstractNodeGeometry &geometry = ngo.nodeScene()->nodeGeometry();

        QJsonDocument json = QJsonDocument::fromVariant(model.nodeData(nodeId, NodeRole::Style));
        NodeStyle nodeStyle(json.object());

        auto const &connectionStyle = StyleCollection::connectionStyle();

        float diameter = nodeStyle.ConnectionPointDiameter;
        auto reducedDiameter = diameter * 0.6;

        for (PortType portType: {PortType::Out, PortType::In}) {
            size_t const n = model
                    .nodeData(nodeId,
                              (portType == PortType::Out) ? NodeRole::OutPortCount
                                                          : NodeRole::InPortCount)
                    .toUInt();

            for (PortIndex portIndex = 0; portIndex < n; ++portIndex) {
                QPointF p = geometry.portPosition(nodeId, portType, portIndex);

                auto const &dataType = model.portData(nodeId, portType, portIndex, PortRole::DataType)
                        .value<NodeDataType>();

                double r = 1.0;

                NodeState const &state = ngo.nodeState();

                if (auto const *cgo = state.connectionForReaction()) {
                    PortType requiredPort = cgo->connectionState().requiredPort();

                    if (requiredPort == portType) {
                        ConnectionId possibleConnectionId = makeCompleteConnectionId(cgo->connectionId(),
                                                                                     nodeId,
                                                                                     portIndex);

                        bool const possible = model.connectionPossible(possibleConnectionId);

                        auto cp = cgo->sceneTransform().map(cgo->endPoint(requiredPort));
                        cp = ngo.sceneTransform().inverted().map(cp);

                        auto diff = cp - p;
                        double dist = std::sqrt(QPointF::dotProduct(diff, diff));

                        if (possible) {
                            double const thres = 40.0;
                            r = (dist < thres) ? (2.0 - dist / thres) : 1.0;
                        } else {
                            double const thres = 80.0;
                            r = (dist < thres) ? (dist / thres) : 1.0;
                        }
                    }
                }

                if (connectionStyle.useDataDefinedColors()) {
                    painter->setBrush(connectionStyle.normalColor(dataType.id));
                } else {
                    painter->setBrush(nodeStyle.ConnectionPointColor);
                }

//                painter->drawEllipse(p, reducedDiameter * r, reducedDiameter * r);
                double expandR = 1.8;
                QPolygon triangle;
                if (portType == PortType::In) {
//                    painter->drawEllipse(p, reducedDiameter * r, reducedDiameter * r);
                    double shiftToRightLength = 0;
                    triangle.setPoints(3,
                                       (int) (p.x() + reducedDiameter * r * expandR + shiftToRightLength),
                                       (int) (p.y()),
                                       (int) (p.x() - reducedDiameter * r * expandR * cos(3.1415 / 3) +
                                              shiftToRightLength),
                                       (int) (p.y() + reducedDiameter * r * expandR * sin(3.1415 / 3)),
                                       (int) (p.x() - reducedDiameter * r * expandR * cos(3.1415 / 3) +
                                              shiftToRightLength),
                                       (int) (p.y() - reducedDiameter * r * expandR * sin(3.1415 / 3)),
                                       true
                    );
                    painter->drawPolygon(triangle);
                } else {
//                    double shiftToLeftLength = 0;
//                    triangle.setPoints(3,
//                                       (int) (p.x() + reducedDiameter * r * expandR - shiftToLeftLength),
//                                       (int) (p.y()),
//                                       (int) (p.x() - reducedDiameter * r * expandR * cos(3.1415 / 3) -
//                                              shiftToLeftLength),
//                                       (int) (p.y() + reducedDiameter * r * expandR * sin(3.1415 / 3)),
//                                       (int) (p.x() - reducedDiameter * r * expandR * cos(3.1415 / 3) -
//                                              shiftToLeftLength),
//                                       (int) (p.y() - reducedDiameter * r * expandR * sin(3.1415 / 3)),
//                                       true
//                    );
//                    painter->drawPolygon(triangle);
                    painter->drawEllipse(p, reducedDiameter * r, reducedDiameter * r);
                }
            }
        }

        if (ngo.nodeState().connectionForReaction()) {
            ngo.nodeState().resetConnectionForReaction();
        }

    }

    void CustomNodePainter::drawFilledConnectionPoints(QPainter *painter, NodeGraphicsObject &ngo) const {
        AbstractGraphModel &model = ngo.graphModel();
        NodeId const nodeId = ngo.nodeId();
        AbstractNodeGeometry &geometry = ngo.nodeScene()->nodeGeometry();

        QJsonDocument json = QJsonDocument::fromVariant(model.nodeData(nodeId, NodeRole::Style));
        NodeStyle nodeStyle(json.object());

        auto diameter = nodeStyle.ConnectionPointDiameter;

        for (PortType portType: {PortType::Out, PortType::In}) {
            size_t const n = model
                    .nodeData(nodeId,
                              (portType == PortType::Out) ? NodeRole::OutPortCount
                                                          : NodeRole::InPortCount)
                    .toUInt();

            for (PortIndex portIndex = 0; portIndex < n; ++portIndex) {
                QPointF p = geometry.portPosition(nodeId, portType, portIndex);

                auto const &connected = model.connections(nodeId, portType, portIndex);

                if (!connected.empty()) {
                    auto const &dataType = model
                            .portData(nodeId, portType, portIndex, PortRole::DataType)
                            .value<NodeDataType>();

                    auto const &connectionStyle = StyleCollection::connectionStyle();
                    if (connectionStyle.useDataDefinedColors()) {
                        QColor const c = connectionStyle.normalColor(dataType.id);
                        painter->setPen(c);
                        painter->setBrush(c);
                    } else {
                        painter->setPen(nodeStyle.FilledConnectionPointColor);
                        painter->setBrush(nodeStyle.FilledConnectionPointColor);
                    }

//                    painter->drawEllipse(p, diameter * 0.4, diameter * 0.4);
                    double expandR = 1.8;
                    QPolygon triangle;
                    if (portType == PortType::In) {
//                    painter->drawEllipse(p, diameter * 0.4, diameter * 0.4);
                        double shiftToRightLength = 0;
                        triangle.setPoints(3,
                                           (int) (p.x() + diameter * 0.4 * expandR + shiftToRightLength),
                                           (int) (p.y()),
                                           (int) (p.x() - diameter * 0.4 * expandR * cos(3.1415 / 3) +
                                                  shiftToRightLength),
                                           (int) (p.y() + diameter * 0.4 * expandR * sin(3.1415 / 3)),
                                           (int) (p.x() - diameter * 0.4 * expandR * cos(3.1415 / 3) +
                                                  shiftToRightLength),
                                           (int) (p.y() - diameter * 0.4 * expandR * sin(3.1415 / 3)),
                                           true
                        );
                    painter->drawPolygon(triangle);
                    } else {
//                        double shiftToLeftLength = 0;
//                        triangle.setPoints(3,
//                                           (int) (p.x() + diameter * 0.4 * expandR - shiftToLeftLength),
//                                           (int) (p.y()),
//                                           (int) (p.x() - diameter * 0.4 * expandR * cos(3.1415 / 3) -
//                                                  shiftToLeftLength),
//                                           (int) (p.y() + diameter * 0.4 * expandR * sin(3.1415 / 3)),
//                                           (int) (p.x() - diameter * 0.4 * expandR * cos(3.1415 / 3) -
//                                                  shiftToLeftLength),
//                                           (int) (p.y() - diameter * 0.4 * expandR * sin(3.1415 / 3)),
//                                           true
//                        );
//                    }
//                    painter->drawPolygon(triangle);
                        painter->drawEllipse(p, diameter * 0.4, diameter * 0.4);
                    }
                }
            }
        }
    }

    void CustomNodePainter::drawNodeCaption(QPainter *painter, NodeGraphicsObject &ngo) const {
        AbstractGraphModel &model = ngo.graphModel();
        NodeId const nodeId = ngo.nodeId();
        AbstractNodeGeometry &geometry = ngo.nodeScene()->nodeGeometry();

        QJsonDocument json = QJsonDocument::fromVariant(model.nodeData(nodeId, NodeRole::Style));
        NodeStyle nodeStyle(json.object());

        auto color = ngo.isSelected()
                     ? nodeStyle.SelectedBoundaryColor
                     : nodeStyle.NormalBoundaryColor;

        if (ngo.nodeState().hovered()) {
            QPen p(nodeStyle.SelectedBoundaryColor, nodeStyle.HoveredPenWidth);
            painter->setPen(p);
        } else {
            QPen p(nodeStyle.NormalBoundaryColor, nodeStyle.PenWidth);
            painter->setPen(p);
        }

//        //Drawing the message background
//        if (model->validationState() == NodeValidationState::Valid) {
//            painter->setBrush(nodeStyle.TitleBarBackgroundColor);
//        } else if (model->validationState() == NodeValidationState::Warning) {
//            painter->setBrush(nodeStyle.TitleBarBackgroundColor);
//        } else if (model->validationState() == NodeValidationState::Error) {
//            painter->setBrush(nodeStyle.TitleBarBackgroundColor);

        double const xRadius = 3.0;
        double const yRadius = 3.0;

        auto geom = dynamic_cast<CustomNodeGeometry&>(geometry);

        QRectF boundary(0,
                        0,
                        geom.m_Width,
                        std::max({32, geom.m_TitleBarHeight}));
        QColor backgroundColor = Qt::transparent;
        painter->setBrush(backgroundColor);
//        painter->drawRoundedRect(boundary, xRadius, yRadius);

        painter->setPen(nodeStyle.FontColor);

        if (!model.nodeData(nodeId, NodeRole::CaptionVisible).toBool())
            return;

        QString const name = model.nodeData(nodeId, NodeRole::Caption).toString();

        QFont f = painter->font();
        f.setBold(true);

        QPointF position = geometry.captionPosition(nodeId);

        painter->setFont(f);
        painter->setPen(nodeStyle.FontColor);
        painter->drawText(position, name);

        f.setBold(false);
        painter->setFont(f);
    }

    void CustomNodePainter::drawEntryLabels(QPainter *painter, NodeGraphicsObject &ngo) const {
        AbstractGraphModel &model = ngo.graphModel();
        NodeId const nodeId = ngo.nodeId();
        AbstractNodeGeometry &geometry = ngo.nodeScene()->nodeGeometry();

        QJsonDocument json = QJsonDocument::fromVariant(model.nodeData(nodeId, NodeRole::Style));
        NodeStyle nodeStyle(json.object());

        for (PortType portType: {PortType::Out, PortType::In}) {
            unsigned int n = model.nodeData<unsigned int>(nodeId,
                                                          (portType == PortType::Out)
                                                          ? NodeRole::OutPortCount
                                                          : NodeRole::InPortCount);

            for (PortIndex portIndex = 0; portIndex < n; ++portIndex) {
                auto const &connected = model.connections(nodeId, portType, portIndex);

                QPointF p = geometry.portTextPosition(nodeId, portType, portIndex);

                if (connected.empty())
                    painter->setPen(nodeStyle.FontColorFaded);
                else
                    painter->setPen(nodeStyle.FontColor);

                QString s;

                if (model.portData<bool>(nodeId, portType, portIndex, PortRole::CaptionVisible)) {
                    s = model.portData<QString>(nodeId, portType, portIndex, PortRole::Caption);
                } else {
                    auto portData = model.portData(nodeId, portType, portIndex, PortRole::DataType);

                    s = portData.value<NodeDataType>().name;
                }

                painter->drawText(p, s);
            }
        }
    }

    void CustomNodePainter::drawResizeRect(QPainter *painter, NodeGraphicsObject &ngo) const {
        AbstractGraphModel &model = ngo.graphModel();
        NodeId const nodeId = ngo.nodeId();
        AbstractNodeGeometry &geometry = ngo.nodeScene()->nodeGeometry();

        if (model.nodeFlags(nodeId) & NodeFlag::Resizable) {
            painter->setBrush(Qt::gray);

            painter->drawEllipse(geometry.resizeHandleRect(nodeId));
        }
    }

    void CustomNodePainter::drawNodeLeftIcon(QPainter *painter, NodeGraphicsObject &ngo) const {
        AbstractGraphModel &model = ngo.graphModel();
        NodeId const nodeId = ngo.nodeId();
        AbstractNodeGeometry &geometry = ngo.nodeScene()->nodeGeometry();

        QJsonDocument json = QJsonDocument::fromVariant(model.nodeData(nodeId, NodeRole::Style));
        NodeStyle nodeStyle(json.object());

        auto color = ngo.isSelected()
                     ? nodeStyle.SelectedBoundaryColor
                     : nodeStyle.NormalBoundaryColor;

        if (ngo.nodeState().hovered()) {
            QPen p(nodeStyle.SelectedBoundaryColor, nodeStyle.HoveredPenWidth);
            painter->setPen(p);
        } else {
            QPen p(nodeStyle.NormalBoundaryColor, nodeStyle.PenWidth);
            painter->setPen(p);
        }

//        //Drawing the message background
//        if (model->validationState() == NodeValidationState::Valid) {
//            painter->setBrush(nodeStyle.TitleBarBackgroundColor);
//        } else if (model->validationState() == NodeValidationState::Warning) {
//            painter->setBrush(nodeStyle.TitleBarBackgroundColor);
//        } else if (model->validationState() == NodeValidationState::Error) {
//            painter->setBrush(nodeStyle.TitleBarBackgroundColor);

        double const xRadius = 3.0;
        double const yRadius = 3.0;

        auto geom = dynamic_cast<CustomNodeGeometry&>(geometry);

        QRectF boundary(0,
                        0,
                        32,
                        32);

        QString iconPath = ":/image/conditions.png";
        QColor backgroundColor = QColor(0xAEEDFF);
        float shrinkRatio = 1.0;
//        if(ngo->connectionState().isConnectionWidgetPressed()){
//            iconPath = ":/image/setting-fill.png";
//            backgroundColor = QColor{0x92ACFF};
//            shrinkRatio = 0.9;
//        }else if(cgo->connectionState().isConnectionWidgetHovered()){
//            iconPath = ":/image/setting-fill.png";
//            backgroundColor = QColor{0xCBEDFF};
//            shrinkRatio = 1.1;
//        }else{
//            iconPath = ":/image/setting-filling.png";
//            backgroundColor = QColor{0xFFFFFF};
//            shrinkRatio = 1.0;
//        }

        QPixmap originalPixmap(iconPath);
        QPixmap pixmap = originalPixmap.scaled(QSize(32, 32)*shrinkRatio);
        QPen p(color, nodeStyle.PenWidth);
        p.setColor(backgroundColor);
        painter->setBrush(backgroundColor);
        painter->setPen(p);
//        painter->drawRoundedRect(boundary, xRadius, yRadius);
        painter->drawPixmap(QPointF(0, 0), pixmap);
    }

    void CustomNodePainter::drawNodeRightIcon(QPainter *painter, NodeGraphicsObject &ngo) const {
        AbstractGraphModel &model = ngo.graphModel();
        NodeId const nodeId = ngo.nodeId();
        AbstractNodeGeometry &geometry = ngo.nodeScene()->nodeGeometry();

        QJsonDocument json = QJsonDocument::fromVariant(model.nodeData(nodeId, NodeRole::Style));
        NodeStyle nodeStyle(json.object());

        auto color = ngo.isSelected()
                     ? nodeStyle.SelectedBoundaryColor
                     : nodeStyle.NormalBoundaryColor;

        if (ngo.nodeState().hovered()) {
            QPen p(color, nodeStyle.HoveredPenWidth);
            painter->setPen(p);
        } else {
            QPen p(color, nodeStyle.PenWidth);
            painter->setPen(p);
        }

//        //Drawing the message background
//        if (model->validationState() == NodeValidationState::Valid) {
//            painter->setBrush(nodeStyle.TitleBarBackgroundColor);
//        } else if (model->validationState() == NodeValidationState::Warning) {
//            painter->setBrush(nodeStyle.TitleBarBackgroundColor);
//        } else if (model->validationState() == NodeValidationState::Error) {
//            painter->setBrush(nodeStyle.TitleBarBackgroundColor);

        double const xRadius = 3.0;
        double const yRadius = 3.0;

        auto geom = dynamic_cast<CustomNodeGeometry&>(geometry);

        QRectF boundary(geom.m_Width - 32,
                        0,
                        32,
                        32);

        QString iconPath = ":/image/conditions.png";
        QColor backgroundColor = QColor(0xAEEDFF);
        float shrinkRatio = 1.0;

        if(ngo.nodeState().getDeleteIconPressed()){
            iconPath = ":/image/setting-fill.png";
            backgroundColor = QColor{0x92ACFF};
            shrinkRatio = 0.7;
        }else if(ngo.nodeState().getIsDeleteIconHovered()){
            iconPath = ":/image/setting-fill.png";
            backgroundColor = QColor{0xCBEDFF};
            shrinkRatio = 0.9;
        }else{
            iconPath = ":/image/setting-filling.png";
            backgroundColor = QColor{0xFFFFFF};
            shrinkRatio = 0.7;
        }

        QPixmap originalPixmap(iconPath);
        QPixmap pixmap = originalPixmap.scaled(QSize(32, 32)*shrinkRatio);
        QPen p(color, nodeStyle.PenWidth);
        p.setColor(backgroundColor);
        painter->setBrush(backgroundColor);
        painter->setPen(p);
//        painter->drawRoundedRect(boundary, xRadius, yRadius);
        painter->drawPixmap(QPointF(geom.m_Width - 32 + (float)(32 - pixmap.size().width())/2, (float)(32 - pixmap.size().width())/2), pixmap);
    }

} // namespace QtNodes
