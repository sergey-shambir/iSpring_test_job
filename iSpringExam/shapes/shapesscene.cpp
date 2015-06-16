
#include "stdafx.h"
#include "shapesscene.h"
#include "ellipsenode.h"
#include "trianglenode.h"
#include "rectanglenode.h"
#include <array>

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
    size_t m_savedHistorySize = 0;
	bool m_didAlteredHistory = false;
    std::pair<int, int> m_size;
    std::pair<int, int> m_minimalSize;
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
    return d->m_didAlteredHistory || (d->m_savedHistorySize != d->m_usedHistorySize);
}

void ShapesScene::doCommand(const AbstractShapeCommandPtr &command)
{
	if (command->redo(*d)) {
		if (d->m_history.size() > d->m_usedHistorySize) {
			d->m_history.resize(d->m_usedHistorySize);
			if (d->m_savedHistorySize > d->m_usedHistorySize)
				d->m_didAlteredHistory = true;
		}
        d->m_history.push_back(command);
        d->m_usedHistorySize = d->m_history.size();
		if (d->m_updateCallback)
			d->m_updateCallback();
    }
}

void ShapesScene::onDidSave()
{
	d->m_didAlteredHistory = false;
	d->m_savedHistorySize = d->m_usedHistorySize;
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
    return true;
}

bool ShapesScene::addFromJsonFile(const std::string &path, std::string &errorReason)
{
	json11::Json json;
	if (FILE *file = fopen(path.c_str(), "r")) {
		std::string jsonContent;
		std::array<char, 64 * 1024> buffer;
		while (size_t len = fread(buffer.data(), sizeof(char), buffer.size(), file))
			jsonContent.append(buffer.data(), len);
		if (ferror(file)) {
			fclose(file);
			errorReason = "Failed to completely read scene file content";
			return false;
		}
		fclose(file);
		json = json11::Json::parse(jsonContent, errorReason);
		if (json.is_null())
			return false;
	} else {
		errorReason = "Failed to read scene file content";
		return false;
	}
	std::string incorrectJson;
	if (!addFromJson(json, incorrectJson)) {
		errorReason = "Scene file corrupted, incorrect JSON part: " + incorrectJson;
		return false;
	}
	return true;
}

bool ShapesScene::saveToJsonFile(const std::string &path, std::string &errorReason) const
{
	if (FILE *file = fopen(path.c_str(), "w")) {
		std::string content;
		json11::Json json{ *this };
		json.dump(content);
		size_t len = fwrite(content.c_str(), sizeof(char), content.size(), file);
		fclose(file);
		if (len != content.size()) {
			errorReason = "Failed to finish writing scene file";
			return false;
		}
	} else {
		errorReason = "Failed to write scene file";
		return false;
	}
	return true;
}

void ShapesScene::render(IVGRenderer &renderer)
{
    for (const auto &node : d->m_nodes)
        node->render(renderer);
}

void ShapesScene::clearScene()
{
    d->m_nodes.clear();
    d->m_history.clear();
    d->m_pickedNode.reset();
    d->m_size = d->m_minimalSize;
	if (d->m_updateCallback)
		d->m_updateCallback();
}

void ShapesScene::setUpdateCallback(const ShapesScene::UpdateCallback &callback)
{
    d->m_updateCallback = callback;
}

bool ShapesScene::pickNode(int x, int y)
{
	vec2 point{ float(x), float(y) };
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
	if (d->m_size.first < d->m_minimalSize.first)
		d->m_size.first = d->m_minimalSize.first;
	if (d->m_size.second < d->m_minimalSize.second)
		d->m_size.second = d->m_minimalSize.second;
}

void ShapesScene::undo()
{
    if (isUndoable()) {
        AbstractShapeCommandPtr command = d->m_history[d->m_usedHistorySize - 1];
        if (command->undo(*d)) {
			--d->m_usedHistorySize;
			d->m_updateCallback();
        }
    }
}

void ShapesScene::redo()
{
    if (isRedoable()) {
        AbstractShapeCommandPtr command = d->m_history[d->m_usedHistorySize];
        if (command->redo(*d)) {
			++d->m_usedHistorySize;
			d->m_updateCallback();
        }
    }
}

bool ShapesScene::isUndoable() const
{
    return d->m_usedHistorySize > 0;
}

bool ShapesScene::isRedoable() const
{
    return d->m_usedHistorySize < d->m_history.size();
}

const NodePtr &ShapesScene::pickedNode() const
{
    return d->m_pickedNode;
}
