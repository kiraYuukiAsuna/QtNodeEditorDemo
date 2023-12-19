#include "ConnectionGeometry.hpp"
#include "ConnectionGraphicsObject.hpp"
#include "PathGenerationAlgorithm.h"

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

    QPainterPath orthogonalPath(ConnectionGraphicsObject* connection){
        QPointF const &in = connection->endPoint(PortType::In);
        QPointF const &out = connection->endPoint(PortType::Out);

        auto points = generateConnectionPoints({in.x(),in.y()},{0,-1},10,{out.x(),out.y()},{0,1},10,0.5);

        QPainterPath path(out);
        QPolygonF polygon;

        for (auto& pair : points) {
            polygon.push_back(QPointF{pair[0],pair[1]});
            path.addPolygon(polygon);
        }

        return path;
    }

}
