#pragma once
#include "abstractnode.h"

class ShapesScene;

class AbstractShapeCommand
{
public:
    virtual ~AbstractShapeCommand() = default;
    virtual bool redo(ShapesScene &scene) = 0;
    virtual bool undo(ShapesScene &scene) = 0;
};

typedef std::shared_ptr<AbstractShapeCommand> AbstractShapeCommandPtr;

class InsertShapeCommand : public AbstractShapeCommand
{
public:
    static const int DEFAULT_SIZE;

    InsertShapeCommand();
    InsertShapeCommand(const std::string &type);

    std::string type() const;
    void setType(const std::string &type);
    bool redo(ShapesScene &scene);
    bool undo(ShapesScene &scene);

private:
    std::string m_type;
    NodePtr m_createdNode;
};

class ResizeShapeCommand
{
public:
};

class MoveShapeCommand
{
public:
};
