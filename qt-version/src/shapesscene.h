#pragma once

#include "abstractnode.h"
#include "shapecommand.h"
#include <gdiplus.h>
#include <list>
#include <memory>
#include <functional>

class ShapesScenePrivate;

class ShapesScene
{
public:
    using UpdateCallback = std::function<void()>;

    ShapesScene();
    ~ShapesScene();

    bool isModified() const;
    json11::Json toJson() const;
    int width() const;
    int height() const;
    const NodePtr &pickedNode() const;

    void doCommand(const AbstractShapeCommandPtr &command);
	void onDidSave();
    bool addFromJson(const json11::Json &json, std::string &incorrectJson);
	bool addFromJsonFile(const std::string &path, std::string &errorReason);
	bool saveToJsonFile(const std::string &path, std::string &errorReason) const;
    void render(AbstractNode::RenderContext &context);
    void clearScene();
    void setUpdateCallback(const UpdateCallback &callback);
    bool pickNode(int x, int y);
    void setMinimalSize(int width, int height);
    void undo();
    void redo();
    bool isUndoable() const;
    bool isRedoable() const;

private:
    std::unique_ptr<ShapesScenePrivate> d;
};
