#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/ConnectionStyle>
#include <QtNodes/GraphicsView>
#include <QtNodes/StyleCollection>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/DataFlowGraphicsScene>
#include <QAction>
#include <QScreen>
#include <QtWidgets/QApplication>
#include "QtNodes/internal/CustomNodePainter.h"
#include "FunctionDelegateModel.h"
#include "PerformanceDelegateModel.h"
#include "ConditionDelegateModel.h"

static std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registerDataModels()
{
    auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();

    ret->registerModel<FunctionDelegateModel>("Function");
    ret->registerModel<PerformanceDelegateModel>("Performance");
    ret->registerModel<ConditionDelegateModel>("Condition");

    return ret;
}


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QtNodes::DataFlowGraphModel graphModel(registerDataModels());

    // Initialize and connect two nodes.
    {
        QtNodes::NodeId id1 = graphModel.addNode("FunctionDelegateModel");
        graphModel.setNodeData(id1, QtNodes::NodeRole::Position, QPointF(0, 0));

        QtNodes::NodeId id2 = graphModel.addNode("PerformanceDelegateModel");
        graphModel.setNodeData(id2, QtNodes::NodeRole::Position, QPointF(300, 300));

        graphModel.addConnection(QtNodes::ConnectionId{id1, 0, id2, 0});
    }

    auto scene = new QtNodes::DataFlowGraphicsScene(graphModel);

    auto newPainter = std::make_unique<QtNodes::CustomNodePainter>();

    scene->setNodePainter(std::move(newPainter));

    QtNodes::GraphicsView view(scene);

    // Setup context menu for creating new nodes.
    view.setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction createNodeAction(QStringLiteral("Create Node"), &view);
    QObject::connect(&createNodeAction, &QAction::triggered, [&]() {
        // Mouse position in scene coordinates.
        QPointF posView = view.mapToScene(view.mapFromGlobal(QCursor::pos()));

        QtNodes::NodeId const newId = graphModel.addNode("ConditionDelegateModel");
        graphModel.setNodeData(newId, QtNodes::NodeRole::Position, posView);
    });
    view.insertAction(view.actions().front(), &createNodeAction);

    view.setWindowTitle("Simple Node Graph");
    view.resize(800, 600);

    // Center window.
    view.move(QApplication::primaryScreen()->availableGeometry().center() - view.rect().center());
    view.showNormal();

    return QApplication::exec();
}
