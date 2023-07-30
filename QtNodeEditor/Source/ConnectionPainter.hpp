#pragma once

#include <QtGui/QPainter>
#include <QtGui/QPainterPath>

#include "Definitions.hpp"

namespace QtNodes {

    class ConnectionGeometry;

    class ConnectionGraphicsObject;

    class ConnectionPainter {
    public:
        static void paint(QPainter *painter, ConnectionGraphicsObject* cgo);

        static QPainterPath getPainterStroke(ConnectionGraphicsObject* cgo);
    };

} // namespace QtNodes
