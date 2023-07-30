#include "ConnectionGeometry.hpp"
#include "ConnectionGraphicsObject.hpp"

namespace QtNodes {

    QPainterPath cubicPath(ConnectionGraphicsObject* connection){
        QPointF const &in = connection->endPoint(PortType::In);
        QPointF const &out = connection->endPoint(PortType::Out);

        auto const c1c2 = connection->pointsC1C2();

        // cubic spline
        QPainterPath cubic(out);

        cubic.cubicTo(c1c2.first, c1c2.second, in);

        return cubic;
    }

}
