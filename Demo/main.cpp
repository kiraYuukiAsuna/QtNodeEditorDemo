#include <QtNodes/ConnectionStyle>
#include <QtNodes/GraphicsView>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/DataFlowGraphicsScene>
#include <QAction>
#include <QScreen>
#include <QtWidgets/QApplication>
#include "QtNodes/internal/CustomNodePainter.h"
#include "FunctionDelegateModel.h"
#include "PerformanceDelegateModel.h"
#include "ConditionDelegateModel.h"
#include <QJsonArray>
#include <QHBoxLayout>
#include <QPushButton>
#include <fstream>
#include <exception>
#include <QtNodes/internal/NodeGraphicsObject.hpp>

static std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registerDataModels()
{
    auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();

    ret->registerModel<FunctionDelegateModel>("Function");
    ret->registerModel<PerformanceDelegateModel>("Performance");
    ret->registerModel<ConditionDelegateModel>("Condition");

    return ret;
}

//std::string internalUUIDConverter(NodeEditorSystem::Node *node) {
//    if (!node) {
//        return "";
//    }
//
//    auto rawuuid = node->id().toString().toStdString();
//    int start = rawuuid.find_first_of("{") + 1;
//    int end = rawuuid.find_first_of("}") - 1;
//    auto uuid = rawuuid.substr(start, end);
//
//    return uuid;
//}

void internalJsonFormatCompactconvertLayer_I2O(QByteArray byteArray, QJsonObject *jsonObject) {
    QJsonDocument internalJsonDocument = QJsonDocument::fromJson(byteArray);
    QJsonObject internalJsonObject = internalJsonDocument.object();

    QJsonArray scenarioJsonArray;
    QJsonArray arrowJsonArray;

    auto convertUUID = [](QJsonValueRef node) {
        auto coveredUUID = node;
        auto coveredUUIDString = coveredUUID.toString().toStdString();

        int start = coveredUUIDString.find_first_of("{") + 1;
        int end = coveredUUIDString.find_first_of("}") - 1;
        auto uuid = coveredUUID.toString().toStdString().substr(start, end);
        return QString::fromStdString(uuid);
    };

    for (QJsonValueRef node: internalJsonObject["nodes"].toArray()) {
        QJsonObject nodeJsonData;

        auto getPosition = [&](std::string coord) {
            auto position = node.toObject()["position"];
            if (coord == "x") {
                return position.toObject()["x"].toDouble();
            } else if (coord == "y") {
                return position.toObject()["y"].toDouble();
            }
//            else if(coord == "z") {
//                return position.toObject()["z"].toDouble();
//            }
            return 0.0;
        };

        nodeJsonData["uuid"] = convertUUID(node.toObject()["id"]);
        nodeJsonData["x"] = getPosition("x");
        nodeJsonData["y"] = getPosition("y");

        scenarioJsonArray.append(nodeJsonData);
    }

    for (QJsonValueRef node: internalJsonObject["connections"].toArray()) {
        QJsonObject nodeJsonData;

        nodeJsonData["startscenario"] = convertUUID(node.toObject()["out_id"]);
        nodeJsonData["endscenario"] = convertUUID(node.toObject()["in_id"]);

        arrowJsonArray.append(nodeJsonData);
    }

    (*jsonObject)["scenarios"] = scenarioJsonArray;
    (*jsonObject)["arrows"] = arrowJsonArray;
}

void internalJsonFormatCompactconvertLayer_O2I(QJsonObject jsonObject, QByteArray *byteArray) {
    QJsonArray internalNodeJsonArray;
    QJsonArray internalConnectionJsonArray;
    QJsonObject internalJsonObject;

    auto convertUUID = [](QJsonValueRef node) {
        auto coveredUUID = node;
        auto uuid = "{" + coveredUUID.toString().toStdString() + "}";
        return QString::fromStdString(uuid);
    };

    for (QJsonValueRef node: jsonObject["scenarios"].toArray()) {
        QJsonObject nodeJsonData;

        nodeJsonData["id"] = convertUUID(node.toObject()["uuid"]);
        QJsonObject position;
        position["x"] = node.toObject()["x"].toDouble();
        position["y"] = node.toObject()["y"].toDouble();
        //position["z"] = node.toObject()["z"].toDouble;
        nodeJsonData["position"] = position;
        QJsonObject model;
        model["name"] = "NodeDataTypeDef::ScenarioNodeDataName";
        nodeJsonData["model"] = model;

        internalNodeJsonArray.append(nodeJsonData);
    }

    for (QJsonValueRef node: jsonObject["arrows"].toArray()) {
        QJsonObject nodeJsonData;

        nodeJsonData["in_id"] = convertUUID(node.toObject()["endscenario"]);
        nodeJsonData["in_index"] = 0;
        nodeJsonData["out_id"] = convertUUID(node.toObject()["startscenario"]);
        nodeJsonData["out_index"] = 0;

        internalConnectionJsonArray.append(nodeJsonData);
    }

    internalJsonObject["nodes"] = internalNodeJsonArray;
    internalJsonObject["connections"] = internalConnectionJsonArray;
    (*byteArray) = QJsonDocument(internalJsonObject).toJson();
}

void loadScene(QtNodes::DataFlowGraphicsScene* scene){
    std::ifstream infile("test_scene_data.json", std::ios::ate | std::ios::binary);

    if (!infile.is_open()) {
        throw std::runtime_error("Failed to open shader file! FilePath: test_scene_data.json");
    }

    uint32_t fileSize = static_cast<uint32_t>(infile.tellg());

    std::vector<char> buffer(fileSize);

    infile.seekg(0);

    infile.read(buffer.data(), fileSize);

    std::string jsonString(buffer.begin(),buffer.end());

    QJsonDocument externalJsonDocument = QJsonDocument::fromJson(QString::fromStdString(jsonString).toUtf8());
    QJsonObject externalJsonObject = externalJsonDocument.object();

//    QByteArray internalByteArray;
//    internalJsonFormatCompactconvertLayer_O2I(externalJsonObject, &internalByteArray);

    scene->clearScene();
    scene->loadFromMemory(externalJsonDocument.toJson());

    auto nodes = scene->graphModel().allNodeIds();

    for (auto &id: nodes) {

    }
}

std::string saveScene(QtNodes::DataFlowGraphicsScene* scene) {
    auto internalByteArray = scene->saveFromMemory();

//    QJsonObject jsonObject;
//    internalJsonFormatCompactconvertLayer_I2O(internalByteArray, &jsonObject);

    std::ofstream of("test_scene_data.json");
    if(!of.is_open()){
        throw std::runtime_error("open file failed!");
    }
    of<<internalByteArray.toStdString();
    of.close();

//    auto jsonString = QJsonDocument(jsonObject).toJson().toStdString();
    return internalByteArray.toStdString();
}

class Listener : public QObject{
    Q_OBJECT
public:
    Listener(QtNodes::DataFlowGraphicsScene* scene) : m_Scene(scene){

    }
public slots:
    void onLoadScene(){
        loadScene(m_Scene);
    }

    void onSaveScene(){
        saveScene(m_Scene);
    }

private:
    QtNodes::DataFlowGraphicsScene* m_Scene;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QtNodes::DataFlowGraphModel graphModel(registerDataModels());

    auto scene = new QtNodes::DataFlowGraphicsScene(graphModel);

    // Initialize and connect two nodes. must after scene and model created!
    {
        QtNodes::NodeId id1 = graphModel.addNode("FunctionDelegateModel");
        graphModel.setNodeData(id1, QtNodes::NodeRole::Position, QPointF(0, 0));

        QtNodes::NodeId id2 = graphModel.addNode("PerformanceDelegateModel");
        graphModel.setNodeData(id2, QtNodes::NodeRole::Position, QPointF(300, 300));

        graphModel.addConnection(QtNodes::ConnectionId{id1, 0, id2, 0}, QJsonObject{});
    }

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

    Listener* listener = new Listener(scene);
    QWidget* controlView = new QWidget;
    QHBoxLayout* controlViewLayout= new QHBoxLayout;
    controlView->setLayout(controlViewLayout);

    auto* loadSceneBtn = new QPushButton;
    loadSceneBtn->setText("Load Scene");
    controlViewLayout->addWidget(loadSceneBtn);
    QObject::connect(loadSceneBtn, &QPushButton::clicked, listener, &Listener::onLoadScene);

    auto* saveSceneBtn = new QPushButton;
    saveSceneBtn->setText("Save Scene");
    controlViewLayout->addWidget(saveSceneBtn);
    QObject::connect(saveSceneBtn, &QPushButton::clicked, listener, &Listener::onSaveScene);

    controlView->show();

    return QApplication::exec();
}

#include "main.moc"
