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

bool InsertShapeCommand::redo(ShapesScene &scene)
{
    if (m_type == TriangleNode::CLASS_NAME)
        m_createdNode = std::make_shared<TriangleNode>();
    else if (m_type == RectangleNode::CLASS_NAME)
        m_createdNode = std::make_shared<RectangleNode>();
    else if (m_type == EllipseNode::CLASS_NAME)
        m_createdNode = std::make_shared<EllipseNode>();
    else
        return false;
    const int halfSize = DEFAULT_SIZE / 2;
    m_createdNode->moveTo(vec2(0.5f * (scene.width() - halfSize), 0.5f * (scene.height() - halfSize)));
    m_createdNode->resizeTo(vec2(DEFAULT_SIZE, DEFAULT_SIZE));
    scene.insertNode(m_createdNode);
    return true;
}

bool InsertShapeCommand::undo(ShapesScene &scene)
{
    (void)scene;
    // TODO: delete m_createdNode
    return false;
}
