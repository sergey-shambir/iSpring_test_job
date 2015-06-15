#include "stdafx.h"
#include "abstractnode.h"

static const int DEFAULT_MINIMAL_SIZE = 40;

const rectangle &AbstractNode::boundingRect() const
{
    return m_boundingRect;
}

vec2 AbstractNode::minimalSize() const
{
    return vec2(float(DEFAULT_MINIMAL_SIZE), float(DEFAULT_MINIMAL_SIZE));
}

void AbstractNode::resizeTo(const vec2 &boundingSize)
{
    m_boundingRect.resizeTo(boundingSize);
}

void AbstractNode::moveTo(const vec2 &toPoint)
{
    m_boundingRect.moveTo(toPoint);
}

void AbstractNode::moveBy(const vec2 &delta)
{
    m_boundingRect.moveBy(delta);
}

void AbstractNode::setBoundingRect(const rectangle &rect)
{
    m_boundingRect = rect;
}
