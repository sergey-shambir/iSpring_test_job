#include "shapesscene.h"
#include "ellipsenode.h"
#include "trianglenode.h"
#include "rectanglenode.h"

class ShapesScenePrivate : public IShapeSceneControl
{
public:
    void insertNode(const NodePtr &node) override;
    void insertNode(const NodePtr &node, int index) override;
    int removeNode(const NodePtr &node) override;
    int width() const;
    int height() const;
    const NodePtr &pickedNode() const;

    std::list<NodePtr> m_nodes;
    std::vector<AbstractShapeCommandPtr> m_history;
    size_t m_usedHistorySize = 0;
    std::pair<int, int> m_size;
    std::pair<int, int> m_minimalSize;
    bool m_isModified = false;
    ShapesScene::UpdateCallback m_updateCallback;
    NodePtr m_pickedNode;
};

void ShapesScenePrivate::insertNode(const NodePtr &node)
{
    m_nodes.push_back(node);
}

void ShapesScenePrivate::insertNode(const NodePtr &node, int index)
{
    auto it = m_nodes.begin();
    while (index != 0) {
        --index;
        ++it;
    }
    m_nodes.insert(it, node);
}

int ShapesScenePrivate::removeNode(const NodePtr &node)
{
    int index = 0;
    for (auto it = m_nodes.begin(), end = m_nodes.end(); it != end; ++it, ++index) {
        if (node == *it) {
            m_nodes.erase(it);
            if (node == m_pickedNode)
                m_pickedNode.reset();
            return index;
        }
    }
    return -1;
}

int ShapesScenePrivate::width() const
{
    return m_size.first;
}

int ShapesScenePrivate::height() const
{
    return m_size.second;
}

const NodePtr &ShapesScenePrivate::pickedNode() const
{
    return m_pickedNode;
}

ShapesScene::ShapesScene()
    : d(new ShapesScenePrivate)
{
}

ShapesScene::~ShapesScene()
{
}

bool ShapesScene::isModified() const
{
    return d->m_isModified;
}

void ShapesScene::doCommand(const AbstractShapeCommandPtr &command)
{
    if (command->redo(*d)) {
        if (d->m_history.size() > d->m_usedHistorySize) {
            d->m_history.resize(d->m_usedHistorySize);
        }
        d->m_history.push_back(command);
        d->m_usedHistorySize = d->m_history.size();
        d->m_updateCallback();
        d->m_isModified = true; // TODO: remember index of command for which scene was unmodified
    }
}

json11::Json ShapesScene::toJson() const
{
    json11::Json::object ret;
    json11::Json::array shapes;
    for (const auto &node : d->m_nodes) {
        shapes.push_back(node->toJson());
    }
    ret["shapes"] = shapes;
    ret["width"] = width();
    ret["height"] = height();
    return json11::Json(ret);
}

int ShapesScene::width() const
{
    return d->m_size.first;
}

int ShapesScene::height() const
{
    return d->m_size.second;
}

bool ShapesScene::addFromJson(const json11::Json &json, std::string &incorrectJson)
{
    auto shapes = json["shapes"].array_items();
    for (const json11::Json &nodeJson : shapes) {
        InsertShapeCommand command;
        command.setType(nodeJson["type"].string_value());
        if (!command.redo(*d) || !d->m_nodes.back()->fromJson(nodeJson)) {
            nodeJson.dump(incorrectJson);
            return false;
        }
    }
    d->m_updateCallback();
    return true;
}

void ShapesScene::render(AbstractNode::RenderContext &context)
{
    for (const auto &node : d->m_nodes) {
        node->render(context);
    }
}

void ShapesScene::clearScene()
{
    d->m_nodes.clear();
    d->m_history.clear();
    d->m_pickedNode.reset();
    d->m_size = d->m_minimalSize;
    d->m_updateCallback();
}

void ShapesScene::setUpdateCallback(const ShapesScene::UpdateCallback &callback)
{
    d->m_updateCallback = callback;
}

bool ShapesScene::pickNode(int x, int y)
{
    vec2 point(x, y);
    for (auto it = d->m_nodes.rbegin(); it != d->m_nodes.rend(); ++it) {
        if ((*it)->testHit(point)) {
            d->m_pickedNode = (*it);
            return true;
        }
    }
    d->m_pickedNode.reset();
    return false;
}

void ShapesScene::setMinimalSize(int width, int height)
{
    d->m_minimalSize.first = width;
    d->m_minimalSize.second = height;
    d->m_size.first = std::max(d->m_size.first, d->m_minimalSize.first);
    d->m_size.second = std::max(d->m_size.second, d->m_minimalSize.second);
}

void ShapesScene::undo()
{
    if (d->m_usedHistorySize > 0) {
        AbstractShapeCommandPtr command = d->m_history[d->m_usedHistorySize - 1];
        if (command->undo(*d)) {
            --d->m_usedHistorySize;
        }
    }
}

void ShapesScene::redo()
{
    if (d->m_usedHistorySize < d->m_history.size()) {
        AbstractShapeCommandPtr command = d->m_history[d->m_usedHistorySize];
        if (command->redo(*d)) {
            ++d->m_usedHistorySize;
        }
    }
    // TODO: implement redo
}

const NodePtr &ShapesScene::pickedNode() const
{
    return d->m_pickedNode;
}
