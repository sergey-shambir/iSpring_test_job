#include "shapesscene.h"
#include "ellipsenode.h"
#include "trianglenode.h"
#include "rectanglenode.h"

ShapesScene::ShapesScene()
{
}

ShapesScene::~ShapesScene()
{
}

bool ShapesScene::isModified() const
{
    return m_isModified;
}

void ShapesScene::doCommand(const AbstractShapeCommandPtr &command)
{
    command->redo(*this);
    m_updateCallback();
}

json11::Json ShapesScene::toJson() const
{
    json11::Json::object ret;
    json11::Json::array shapes;
    for (const auto &node : m_nodes) {
        shapes.push_back(node->toJson());
    }
    ret["shapes"] = shapes;
    ret["width"] = m_size.first;
    ret["height"] = m_size.second;
    return json11::Json(ret);
}

int ShapesScene::width() const
{
    return m_size.first;
}

int ShapesScene::height() const
{
    return m_size.second;
}

bool ShapesScene::addFromJson(const json11::Json &json, std::string &incorrectJson)
{
    auto shapes = json["shapes"].array_items();
    InsertShapeCommand command;
    for (const json11::Json &nodeJson : shapes) {
        command.setType(nodeJson["type"].string_value());
        if (!command.redo(*this) || !m_nodes.back()->fromJson(nodeJson)) {
            nodeJson.dump(incorrectJson);
            return false;
        }
    }
    m_updateCallback();
    return true;
}

void ShapesScene::render(AbstractNode::RenderContext &context)
{
    for (const auto &node : m_nodes) {
        node->render(context);
    }
}

void ShapesScene::clearScene()
{
    m_nodes.clear();
    m_history.clear();
    m_pickedNode.reset();
    m_size = m_minimalSize;
    m_updateCallback();
}

void ShapesScene::setUpdateCallback(const ShapesScene::UpdateCallback &callback)
{
    m_updateCallback = callback;
}

bool ShapesScene::pickNode(int x, int y)
{
    vec2 point(x, y);
    for (auto it = m_nodes.rbegin(); it != m_nodes.rend(); ++it) {
        if ((*it)->testHit(point)) {
            m_pickedNode = (*it);
            return true;
        }
    }
    m_pickedNode.reset();
    return false;
}

void ShapesScene::setMinimalSize(int width, int height)
{
    m_minimalSize.first = width;
    m_minimalSize.second = height;
    m_size.first = std::max(m_size.first, m_minimalSize.first);
    m_size.second = std::max(m_size.second, m_minimalSize.second);
}

const NodePtr &ShapesScene::pickedNode() const
{
    return m_pickedNode;
}

void ShapesScene::insertNode(const NodePtr &node)
{
    m_nodes.push_back(node);
}
