#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_GraphModel(registerDataModels()){
    ui->setupUi(this);



    m_Scene = new QtNodes::DataFlowGraphicsScene(m_GraphModel);

    // Initialize and connect two nodes. must after scene and model created!
    {
        QtNodes::NodeId id1 = m_GraphModel.addNode("FunctionDelegateModel");
        m_GraphModel.setNodeData(id1, QtNodes::NodeRole::Position, QPointF(0, 0));

        QtNodes::NodeId id2 = m_GraphModel.addNode("PerformanceDelegateModel");
        m_GraphModel.setNodeData(id2, QtNodes::NodeRole::Position, QPointF(300, 300));

        m_GraphModel.addConnection(QtNodes::ConnectionId{id1, 0, id2, 0}, QJsonObject{});
    }

    auto newPainter = std::make_unique<QtNodes::CustomNodePainter>();

    m_Scene->setNodePainter(std::move(newPainter));

    m_View.setScene(m_Scene);

    // Setup context menu for creating new nodes.
    m_View.setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction createNodeAction(QStringLiteral("Create Node"), &m_View);
    QObject::connect(&createNodeAction, &QAction::triggered, [&]() {
        // Mouse position in scene coordinates.
        QPointF posView = m_View.mapToScene(m_View.mapFromGlobal(QCursor::pos()));

        QtNodes::NodeId const newId = m_GraphModel.addNode("ConditionDelegateModel");
        m_GraphModel.setNodeData(newId, QtNodes::NodeRole::Position, posView);
    });
    m_View.insertAction(m_View.actions().front(), &createNodeAction);

    m_View.setWindowTitle("Simple Node Graph");
    m_View.resize(800, 600);

    // Center window.
    m_View.move(QApplication::primaryScreen()->availableGeometry().center() - m_View.rect().center());
    m_View.showNormal();

    m_Listener = new Listener(m_Scene);
    QWidget* controlView = new QWidget;
    QHBoxLayout* controlViewLayout = new QHBoxLayout;
    controlView->setLayout(controlViewLayout);

    auto* loadSceneBtn = new QPushButton;
    loadSceneBtn->setText("Load Scene");
    controlViewLayout->addWidget(loadSceneBtn);
    QObject::connect(loadSceneBtn, &QPushButton::clicked, m_Listener, &Listener::onLoadScene);

    auto* saveSceneBtn = new QPushButton;
    saveSceneBtn->setText("Save Scene");
    controlViewLayout->addWidget(saveSceneBtn);
    QObject::connect(saveSceneBtn, &QPushButton::clicked, m_Listener, &Listener::onSaveScene);

    controlView->show();
}

MainWindow::~MainWindow() {
    delete ui;
}
