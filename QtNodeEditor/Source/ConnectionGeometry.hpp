#pragma once

#include <QSize>
#include <QPointF>
#include <QPainterPath>

namespace QtNodes {

    class ConnectionGraphicsObject;

    QPainterPath cubicPath(ConnectionGraphicsObject* connection);

    class ConnectionGeometry{
    public:
        ConnectionGeometry(ConnectionGraphicsObject* ConnectionGraphicsObject) : m_ConnectionGraphicsObject(ConnectionGraphicsObject){

        }

        void updateGeometry(){
            auto cubic = cubicPath(m_ConnectionGraphicsObject);

            m_ConditionWidgetRect = {cubic.pointAtPercent(0.50) - QPoint(m_IconSize.width() / 2, m_IconSize.height() / 2),
                                     QSize(m_IconSize.width(), m_IconSize.height())};

            m_IconSize = {24,24};
            m_IconPosition = cubic.pointAtPercent(0.50) - QPoint(m_IconSize.width() / 2, m_IconSize.height() / 2);
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
