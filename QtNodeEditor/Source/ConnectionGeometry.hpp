#pragma once

#include <QSize>
#include <QPointF>
#include <QPainterPath>

namespace QtNodes {

    class ConnectionGraphicsObject;

    QPainterPath cubicPath(ConnectionGraphicsObject* connection);
    QPainterPath orthogonalPath(ConnectionGraphicsObject* connection);

    class ConnectionGeometry{
    public:
        ConnectionGeometry(ConnectionGraphicsObject* ConnectionGraphicsObject) : m_ConnectionGraphicsObject(ConnectionGraphicsObject){

        }

        void updateGeometry(){
            auto path = orthogonalPath(m_ConnectionGraphicsObject);

            m_ConditionWidgetRect = {path.pointAtPercent(0.50) - QPoint(m_IconSize.width() / 2, m_IconSize.height() / 2),
                                     QSize(m_IconSize.width(), m_IconSize.height())};

            m_IconSize = {64,64};
            m_IconPosition = path.pointAtPercent(0.50) - QPoint(m_IconSize.width() / 2, m_IconSize.height() / 2);
        }

        QRectF getShrinkConditionWidgetRect(float shrinkRatio){
            auto path = orthogonalPath(m_ConnectionGraphicsObject);
            return {path.pointAtPercent(0.50) - QPoint(m_IconSize.width() * shrinkRatio / 2, m_IconSize.height() * shrinkRatio / 2),
                                     QSize(m_IconSize.width() * shrinkRatio, m_IconSize.height() * shrinkRatio)};
        }

        QSize getShrinkIconSize(float shrinkRatio){
            return m_IconSize * shrinkRatio;
        }

        QPointF getShrinkIconPosition(float shrinkRatio){
            auto path = orthogonalPath(m_ConnectionGraphicsObject);
            return path.pointAtPercent(0.50) - QPoint(m_IconSize.width() * shrinkRatio / 2, m_IconSize.height() * shrinkRatio / 2);
        }

        QRectF getConditionWidgetRect() {
            return m_ConditionWidgetRect;
        }

        QSize getIconSize() {
            return m_IconSize;
        }

        QPointF getIconPosition() {
            return m_IconPosition;
        }

    private:
        QRectF m_ConditionWidgetRect;

        QSize m_IconSize;
        QPointF m_IconPosition;

        ConnectionGraphicsObject* m_ConnectionGraphicsObject;
    };

}
