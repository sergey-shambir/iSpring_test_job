#include "shapecommand.h"
#include "trianglenode.h"
#include "rectanglenode.h"
#include "ellipsenode.h"
#include "shapesscene.h"

const int InsertShapeCommand::DEFAULT_SIZE = 100;

InsertShapeCommand::InsertShapeCommand() = default;

InsertShapeCommand::InsertShapeCommand(const std::string &type)
    : m_type(type)
{
}

std::string InsertShapeCommand::type() const
{
    return m_type;
}

void InsertShapeCommand::setType(const std::string &type)
{
    m_type = type;
}

bool InsertShapeCommand::redo(IShapeSceneControl &scene)
{
    if (!m_createdNode) {
        if (m_type == TriangleNode::CLASS_NAME)
            m_createdNode = std::make_shared<TriangleNode>();
        else if (m_type == RectangleNode::CLASS_NAME)
            m_createdNode = std::make_shared<RectangleNode>();
        else if (m_type == EllipseNode::CLASS_NAME)
            m_createdNode = std::make_shared<EllipseNode>();
        else
            return false;
    }
    const int halfSize = DEFAULT_SIZE / 2;
    m_createdNode->moveTo(vec2(0.5f * (scene.width() - halfSize), 0.5f * (scene.height() - halfSize)));
    m_createdNode->resizeTo(vec2(DEFAULT_SIZE, DEFAULT_SIZE));
    scene.insertNode(m_createdNode);
    return true;
}

bool InsertShapeCommand::undo(IShapeSceneControl &scene)
{
    if (m_createdNode) {
        scene.removeNode(m_createdNode);
        return true;
    }
    return false;
}

SetShapeBoundsCommand::SetShapeBoundsCommand()
{
}

SetShapeBoundsCommand::SetShapeBoundsCommand(const rectangle &newBounds)
    : m_newBounds(newBounds)
{
}

rectangle SetShapeBoundsCommand::newBounds() const
{
    return m_newBounds;
}

void SetShapeBoundsCommand::setNewBounds(const rectangle &newBounds)
{
    m_newBounds = newBounds;
}

bool SetShapeBoundsCommand::redo(IShapeSceneControl &scene)
{
    if (!m_targetNode) {
        m_targetNode = scene.pickedNode();
        if (!m_targetNode)
            return false;
    }
    m_oldBounds = m_targetNode->boundingRect();
    m_targetNode->moveTo(m_newBounds.origin());
    m_targetNode->resizeTo(m_newBounds.size());
    return true;
}

bool SetShapeBoundsCommand::undo(IShapeSceneControl &scene)
{
    (void)scene;
    if (m_targetNode) {
        m_targetNode->moveTo(m_oldBounds.origin());
        m_targetNode->resizeTo(m_oldBounds.size());
        return true;
    }
    return false;
}

DeleteShapeCommand::DeleteShapeCommand() = default;

bool DeleteShapeCommand::redo(IShapeSceneControl &scene)
{
    if (!m_targetNode) {
        m_targetNode = scene.pickedNode();
        if (!m_targetNode)
            return false;
    }
    m_nodeIndex = scene.removeNode(m_targetNode);
    return true;
}

bool DeleteShapeCommand::undo(IShapeSceneControl &scene)
{
    if (m_targetNode) {
        scene.insertNode(m_targetNode, m_nodeIndex);
        return true;
    }
    return false;
}
