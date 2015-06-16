#pragma once
#include "abstractnode.h"

class ShapesScene;

class IShapeSceneControl
{
public:
    virtual ~IShapeSceneControl() = default;
    virtual void insertNode(const NodePtr &node) = 0;
    virtual void insertNode(const NodePtr &node, int index) = 0;
    virtual int removeNode(const NodePtr &node) = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual const NodePtr &pickedNode() const = 0;
};

class AbstractShapeCommand
{
public:
    virtual ~AbstractShapeCommand() = default;
    virtual bool redo(IShapeSceneControl &scene) = 0;
    virtual bool undo(IShapeSceneControl &scene) = 0;
};

typedef std::shared_ptr<AbstractShapeCommand> AbstractShapeCommandPtr;

class InsertShapeCommand : public AbstractShapeCommand
{
public:
    static const int DEFAULT_SIZE;

    InsertShapeCommand();
    explicit InsertShapeCommand(const std::string &type);
    std::string type() const;
    void setType(const std::string &type);
    bool redo(IShapeSceneControl &scene);
    bool undo(IShapeSceneControl &scene);

private:
    std::string m_type;
    NodePtr m_createdNode;
};

class SetShapeBoundsCommand : public AbstractShapeCommand
{
public:
    SetShapeBoundsCommand();
    explicit SetShapeBoundsCommand(const rectangle &newBounds);
    rectangle newBounds() const;
    void setNewBounds(const rectangle &newBounds);
    bool redo(IShapeSceneControl &scene);
    bool undo(IShapeSceneControl &scene);

private:
    rectangle m_newBounds;
    rectangle m_oldBounds;
    NodePtr m_targetNode;
};

class DeleteShapeCommand : public AbstractShapeCommand
{
public:
    DeleteShapeCommand();
    bool redo(IShapeSceneControl &scene);
    bool undo(IShapeSceneControl &scene);

private:
    int m_nodeIndex = 0;
    NodePtr m_targetNode;
};
